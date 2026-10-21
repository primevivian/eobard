// Link between both halves over PIO UART
// Both halves always send, the one on USB uses what it gets
// Frame is SPLIT_SYNC, keys[7:0], keys[15:8], keys[23:16], checksum

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "uart_tx.pio.h"
#include "uart_rx.pio.h"

#include "config.h"
#include "split.h"

#define SPLIT_SYNC        0xA5
#define SPLIT_FRAME_LEN   5

static PIO tx_pio;
static uint tx_sm;
static uint tx_offset;

static PIO rx_pio;
static uint rx_sm;
static uint rx_offset;

void split_init(void)
{
  // This will find a free pio and state machine for our program and load it for us
  bool success = pio_claim_free_sm_and_add_program_for_gpio_range(&uart_tx_program, &tx_pio, &tx_sm, &tx_offset, SPLIT_TX_PIN, 1, true);
  hard_assert(success);
  uart_tx_program_init(tx_pio, tx_sm, tx_offset, SPLIT_TX_PIN, SPLIT_BAUD);

  success = pio_claim_free_sm_and_add_program_for_gpio_range(&uart_rx_program, &rx_pio, &rx_sm, &rx_offset, SPLIT_RX_PIN, 1, true);
  hard_assert(success);
  uart_rx_program_init(rx_pio, rx_sm, rx_offset, SPLIT_RX_PIN, SPLIT_BAUD);
}

static uint8_t split_checksum(uint8_t const* data)
{
  return (uint8_t) ((data[0] + data[1] + data[2]) ^ 0x5A);
}

static void split_send(uint32_t keys)
{
  uint8_t frame[SPLIT_FRAME_LEN] = { SPLIT_SYNC, (uint8_t) keys, (uint8_t) (keys >> 8), (uint8_t) (keys >> 16), 0 };
  frame[4] = split_checksum(&frame[1]);

  for (uint i = 0; i < SPLIT_FRAME_LEN; i++) uart_tx_program_putc(tx_pio, tx_sm, (char) frame[i]);
}

// Read what has arrived, true once a valid frame is in
static bool split_receive(uint32_t* keys)
{
  static uint8_t frame[SPLIT_FRAME_LEN];
  static uint len = 0;
  bool received = false;

  while (!pio_sm_is_rx_fifo_empty(rx_pio, rx_sm))
  {
    uint8_t const b = (uint8_t) uart_rx_program_getc(rx_pio, rx_sm);

    // Skip until a frame starts
    if (len == 0 && b != SPLIT_SYNC) continue;

    frame[len++] = b;
    if (len < SPLIT_FRAME_LEN) continue;

    len = 0;
    if (frame[4] == split_checksum(&frame[1]))
    {
      *keys = frame[1] | (frame[2] << 8) | ((uint32_t) frame[3] << 16);
      received = true;
    }
  }

  return received;
}

uint32_t split_task(uint32_t local_keys, uint32_t now_ms)
{
  static uint32_t last_sent_keys = 0;
  static uint32_t last_sent_ms = 0;
  static uint32_t remote_keys = 0;
  static uint32_t last_received_ms = 0;
  static bool connected = false;

  // Send on change, otherwise as keepalive
  if (local_keys != last_sent_keys || now_ms - last_sent_ms >= SPLIT_KEEPALIVE_MS)
  {
    split_send(local_keys);
    last_sent_keys = local_keys;
    last_sent_ms = now_ms;
  }

  if (split_receive(&remote_keys))
  {
    last_received_ms = now_ms;
    if (!connected) DEBUG_PRINT("split: other half connected\n");
    connected = true;
  }else if (connected && now_ms - last_received_ms >= SPLIT_TIMEOUT_MS)
  {
    // Release everything so no keys get stuck
    DEBUG_PRINT("split: other half lost\n");
    connected = false;
    remote_keys = 0;
  }

  return remote_keys;
}
