// Key matrix scanning for the diode prototype

#include "pico/stdlib.h"

#include "config.h"
#include "matrix.h"

static const uint row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const uint col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

void matrix_init(void)
{
  // Rows read high until a key pulls them low
  for (uint r = 0; r < MATRIX_ROWS; r++)
  {
    gpio_init(row_pins[r]);
    gpio_set_dir(row_pins[r], GPIO_IN);
    gpio_pull_up(row_pins[r]);
  }

  // Columns sit pulled up until scanned, only one driven at a time
  for (uint c = 0; c < MATRIX_COLS; c++)
  {
    gpio_init(col_pins[c]);
    gpio_set_dir(col_pins[c], GPIO_IN);
    gpio_pull_up(col_pins[c]);
    gpio_put(col_pins[c], 0);
  }
}

// Wait for rows to go back high so the next column reads clean
static void matrix_wait_rows_high(void)
{
  for (uint t = 0; t < 50; t++) // give up after ~50us
  {
    bool all_high = true;
    for (uint r = 0; r < MATRIX_ROWS; r++) all_high &= gpio_get(row_pins[r]);
    if (all_high) return;
    sleep_us(1);
  }
}

static uint32_t matrix_read(void)
{
  uint32_t keys = 0;

  for (uint c = 0; c < MATRIX_COLS; c++)
  {
    // Drive column low, held keys pull their row low through the diode
    gpio_set_dir(col_pins[c], GPIO_OUT);
    sleep_us(5); // settle time

    for (uint r = 0; r < MATRIX_ROWS; r++)
    {
      if (!gpio_get(row_pins[r])) keys |= 1u << (r * MATRIX_COLS + c);
    }

    // Release column back to its pull-up, a floating column ghosts into the next one
    gpio_set_dir(col_pins[c], GPIO_IN);
    matrix_wait_rows_high();
  }

  return keys;
}

uint32_t matrix_scan(uint32_t now_ms)
{
  static uint32_t debounced = 0;
  static uint32_t last_raw = 0;
  static uint32_t last_change_ms = 0;

  uint32_t const raw = matrix_read();

  if (raw != last_raw)
  {
    last_raw = raw;
    last_change_ms = now_ms;
  }else if (now_ms - last_change_ms >= DEBOUNCE_MS)
  {
    debounced = raw;
  }

  return debounced;
}

uint64_t matrix_to_positions(uint32_t left_keys, uint32_t right_keys)
{
  uint64_t positions = 0;

  for (uint r = 0; r < MATRIX_ROWS; r++)
  {
    for (uint c = 0; c < MATRIX_COLS; c++)
    {
      uint const key = r * MATRIX_COLS + c;
      uint const row_start = r * MATRIX_COLS * 2;

      // Left half takes the first five columns of each row
      if (left_keys & (1u << key)) positions |= 1ull << (row_start + c);

      // Right half is the same PCB flipped, so its columns run backwards
      if (right_keys & (1u << key)) positions |= 1ull << (row_start + MATRIX_COLS * 2 - 1 - c);
    }
  }

  return positions;
}
