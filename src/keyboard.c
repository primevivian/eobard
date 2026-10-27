// Key presses into HID reports, handles layers, layer taps and modifiers
// Layer taps work like KMK defaults, tap under TAPPING_TERM_MS, hold over it,
// any other key pressed while undecided makes it a hold

#include <string.h>

#include "pico/bootrom.h"

#include "keyboard.h"

#define REPORT_QUEUE_SIZE  16

// layer_state has a bit per layer
_Static_assert(NUM_LAYERS <= 8, "too many layers for layer_state");

static uint8_t layer_state = 1u << 0;         // bit per active layer, base always on
static keycode_t pressed_as[NUM_KEYS];        // what each held position was on press
static keycode_t registered[NUM_KEYS];        // what each position is sending right now

// Layer tap held but not yet a tap or hold
static struct
{
  bool active;
  uint8_t pos;
  uint32_t press_ms;
} pending_lt;

// BOOTSEL key held but not long enough to reboot
static struct
{
  bool active;
  uint32_t press_ms;
} pending_bootsel;

static hid_keyboard_report_t report_queue[REPORT_QUEUE_SIZE];
static uint report_head = 0;
static uint report_count = 0;

/// Reports

// Build a report from everything registered, queue it if it changed
static void queue_report(void)
{
  static hid_keyboard_report_t last_report = { 0 };
  hid_keyboard_report_t report = { 0 };
  uint count = 0;

  for (uint pos = 0; pos < NUM_KEYS; pos++)
  {
    keycode_t const kc = registered[pos];
    uint8_t const usage = KEY_USAGE(kc);

    report.modifier |= KEY_MODS(kc);

    if (usage >= HID_KEY_CONTROL_LEFT && usage <= HID_KEY_GUI_RIGHT)
    {
      report.modifier |= 1u << (usage - HID_KEY_CONTROL_LEFT);
    }else if (usage != HID_KEY_NONE && count < 6)
    {
      report.keycode[count++] = usage;
    }
  }

  if (memcmp(&report, &last_report, sizeof(report)) == 0) return;

  if (report_count == REPORT_QUEUE_SIZE)
  {
    // Host isn't reading, overwrite the newest report so it still ends on the real key state
    DEBUG_PRINT("keyboard: report queue full, replacing newest report\n");
    report_queue[(report_head + report_count - 1) % REPORT_QUEUE_SIZE] = report;
  }else
  {
    report_queue[(report_head + report_count) % REPORT_QUEUE_SIZE] = report;
    report_count++;
  }

  last_report = report;
}

bool keyboard_next_report(hid_keyboard_report_t* report)
{
  if (report_count == 0) return false;

  *report = report_queue[report_head];
  report_head = (report_head + 1) % REPORT_QUEUE_SIZE;
  report_count--;
  return true;
}

/// Layers

// Key on the highest active layer that isn't transparent
static keycode_t keymap_lookup(uint pos)
{
  for (int layer = NUM_LAYERS - 1; layer >= 0; layer--)
  {
    if (!(layer_state & (1u << layer))) continue;

    keycode_t const kc = keymap[layer][pos];
    if (kc != KEY_TRANSPARENT) return kc;
  }

  return HID_KEY_NONE;
}

static void layer_on(uint8_t layer)
{
  layer_state |= 1u << layer;
  DEBUG_PRINT("layer: %u on\n", layer);
}

static void layer_off(uint8_t layer)
{
  if (layer == 0) return;
  layer_state &= ~(1u << layer);
  DEBUG_PRINT("layer: %u off\n", layer);
}

// Pending layer tap becomes a hold
static void pending_lt_hold(void)
{
  pending_lt.active = false;
  layer_on(KEY_LT_LAYER(pressed_as[pending_lt.pos]));
}

/// Key Events

// Mods of shifted symbols only apply to the symbol itself,
// drop them from held keys so they don't shift the next one
static void clear_symbol_mods(void)
{
  for (uint pos = 0; pos < NUM_KEYS; pos++)
  {
    // Keys without a usage are pure modifiers like KEY_HYPER, those stay
    if (KEY_USAGE(registered[pos]) != HID_KEY_NONE) registered[pos] = KEY_USAGE(registered[pos]);
  }
}

static void key_pressed(uint pos, uint32_t now_ms)
{
  // Another key interrupts an undecided layer tap, make it a hold
  if (pending_lt.active) pending_lt_hold();

  clear_symbol_mods();

  keycode_t const kc = keymap_lookup(pos);
  pressed_as[pos] = kc;

  DEBUG_PRINT("key: %u down (0x%04x)\n", pos, kc);

  if (KEY_IS_LT(kc))
  {
    pending_lt.active = true;
    pending_lt.pos = (uint8_t) pos;
    pending_lt.press_ms = now_ms;
  }else if (kc == KEY_BOOTSEL)
  {
    // Only reboots once held, so rolling onto it can't trigger it
    pending_bootsel.active = true;
    pending_bootsel.press_ms = now_ms;
  }else
  {
    registered[pos] = kc;
    queue_report();
  }
}

static void key_released(uint pos)
{
  keycode_t const kc = pressed_as[pos];
  pressed_as[pos] = HID_KEY_NONE;

  DEBUG_PRINT("key: %u up\n", pos);

  if (KEY_IS_LT(kc))
  {
    if (pending_lt.active && pending_lt.pos == pos)
    {
      // Released while undecided, send the tap
      pending_lt.active = false;
      registered[pos] = KEY_USAGE(kc);
      queue_report();
      registered[pos] = HID_KEY_NONE;
      queue_report();
    }else
    {
      layer_off(KEY_LT_LAYER(kc));
    }
  }else if (kc == KEY_BOOTSEL)
  {
    pending_bootsel.active = false;
  }else
  {
    registered[pos] = HID_KEY_NONE;
    queue_report();
  }
}

void keyboard_task(uint64_t keys, uint32_t now_ms)
{
  static uint64_t last_keys = 0;

  // Held past the tapping term, make it a hold
  if (pending_lt.active && now_ms - pending_lt.press_ms >= TAPPING_TERM_MS) pending_lt_hold();

  if (pending_bootsel.active && now_ms - pending_bootsel.press_ms >= BOOTSEL_HOLD_MS)
  {
    DEBUG_PRINT("keyboard: rebooting into BOOTSEL\n");
    reset_usb_boot(0, 0);
  }

  uint64_t const released = last_keys & ~keys;
  uint64_t const pressed = keys & ~last_keys;

  // Changes arrive in batches, do releases first since a release and press
  // in one batch is nearly always a typing roll
  for (uint pos = 0; pos < NUM_KEYS; pos++)
  {
    if (released & (1ull << pos)) key_released(pos);
  }

  for (uint pos = 0; pos < NUM_KEYS; pos++)
  {
    if (pressed & (1ull << pos)) key_pressed(pos, now_ms);
  }

  last_keys = keys;
}
