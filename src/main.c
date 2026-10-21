// Eobard split keyboard firmware
// By primevivian 2026

// Based on dev_hid_composite from pico-examples
// Both halves run this loop, the one on USB sends both halves' keys to the host

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "tusb.h"

#include "config.h"
#include "keyboard.h"
#include "matrix.h"
#include "split.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+

void key_task(void);
void hid_task(void);

static inline uint32_t board_millis(void)
{
  return to_ms_since_boot(get_absolute_time());
}

/*------------- MAIN -------------*/
int main(void)
{
  stdio_init_all();
  DEBUG_PRINT("Starting Eobard (%s half)\n", EOBARD_LEFT ? "left" : "right");

  // Onboard LED shows caps lock
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  matrix_init();
  split_init();

  // init device stack on configured roothub port
  const tusb_rhport_init_t rh_init = {
    .role = TUSB_ROLE_DEVICE,
    .speed = TUD_OPT_HIGH_SPEED ? TUSB_SPEED_HIGH : TUSB_SPEED_FULL
  };
  TU_ASSERT(tud_rhport_init(BOARD_TUD_RHPORT, &rh_init));

  while (1)
  {
    tud_task(); // tinyusb device task
    key_task();
    hid_task();
  }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  DEBUG_PRINT("usb: mounted\n");
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  DEBUG_PRINT("usb: unmounted\n");
}

//--------------------------------------------------------------------+
// KEYS
//--------------------------------------------------------------------+

// Every 1ms, scan this half, swap keys with the other half, process them
void key_task(void)
{
  const uint32_t interval_ms = 1;
  static uint32_t start_ms = 0;

  uint32_t const now_ms = board_millis();
  if ( now_ms - start_ms < interval_ms) return; // not enough time
  start_ms = now_ms;

  uint32_t const local_keys = matrix_scan(now_ms);
  uint32_t const remote_keys = split_task(local_keys, now_ms);

  // Only the half on USB makes reports
  if ( !tud_mounted() ) return;

  uint64_t const keys = EOBARD_LEFT ? matrix_to_positions(local_keys, remote_keys)
                                    : matrix_to_positions(remote_keys, local_keys);

  // Remote wakeup
  if ( tud_suspended() && keys )
  {
    // Wake up host if we are in suspend mode
    // and REMOTE_WAKEUP feature is enabled by host
    tud_remote_wakeup();
  }

  keyboard_task(keys, now_ms);
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// Send the next queued report once the last one is out
void hid_task(void)
{
  // skip if hid is not ready yet
  if ( !tud_hid_ready() ) return;

  hid_keyboard_report_t report;
  if ( keyboard_next_report(&report) )
  {
    tud_hid_keyboard_report(REPORT_ID_KEYBOARD, report.modifier, report.keycode);
  }
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    // Set keyboard LED e.g Capslock, Numlock etc...
    if (report_id == REPORT_ID_KEYBOARD)
    {
      // bufsize should be (at least) 1
      if ( bufsize < 1 ) return;

      uint8_t const kbd_leds = buffer[0];

      // LED on while caps lock is on
      gpio_put(PICO_DEFAULT_LED_PIN, kbd_leds & KEYBOARD_LED_CAPSLOCK);
    }
  }
}
