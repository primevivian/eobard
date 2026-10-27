// Switch scanning for the diodeless PCB, every switch on its own pin

#include "pico/stdlib.h"

#include "config.h"
#include "matrix.h"

// Split frame carries 24 bits of keys per half
_Static_assert(NUM_SWITCHES <= 24, "too many switches for the split frame");
_Static_assert(NUM_SWITCHES <= KEYMAP_ROWS * KEYMAP_COLS, "too many switches for the keymap");

static const uint switch_pins[NUM_SWITCHES] = SWITCH_PINS;

void matrix_init(void)
{
  // Pins read high until a switch pulls them to GND
  for (uint sw = 0; sw < NUM_SWITCHES; sw++)
  {
    gpio_init(switch_pins[sw]);
    gpio_set_dir(switch_pins[sw], GPIO_IN);
    gpio_pull_up(switch_pins[sw]);
  }
}

static uint32_t matrix_read(void)
{
  uint32_t keys = 0;

  for (uint sw = 0; sw < NUM_SWITCHES; sw++)
  {
    if (!gpio_get(switch_pins[sw])) keys |= 1u << sw;
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

  for (uint sw = 0; sw < NUM_SWITCHES; sw++)
  {
    uint const c = sw % KEYMAP_COLS;
    uint const row_start = (sw / KEYMAP_COLS) * KEYMAP_COLS * 2;

    // Left half takes the first five columns of each row
    if (left_keys & (1u << sw)) positions |= 1ull << (row_start + c);

    // Right half is the same PCB flipped, so its columns run backwards
    if (right_keys & (1u << sw)) positions |= 1ull << (row_start + KEYMAP_COLS * 2 - 1 - c);
  }

  return positions;
}
