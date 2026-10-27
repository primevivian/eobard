// Key presses into HID reports, handles layers, layer taps and modifiers

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

#include "tusb.h"

#include "config.h"

#define NUM_LAYERS  7

/// Keycodes

// Keymap entries are 16 bits
// 0x0000 - 0x0FFF HID key, bits 0-7 HID usage, bits 8-11 left modifiers held with it
// 0x1000 - 0x1FFF layer tap, bits 0-7 HID usage on tap, bits 8-11 layer on hold
// 0x2000 and up special keys
typedef uint16_t keycode_t;

#define MODS(mods, usage)   ((keycode_t) (((mods) << 8) | (usage)))
#define LSFT(usage)         MODS(KEYBOARD_MODIFIER_LEFTSHIFT, usage)
#define LT(layer, usage)    ((keycode_t) (0x1000 | ((layer) << 8) | (usage)))

#define KEY_USAGE(kc)       ((uint8_t) ((kc) & 0xFF))
#define KEY_MODS(kc)        ((uint8_t) (((kc) >> 8) & 0x0F))
#define KEY_IS_LT(kc)       (((kc) & 0xF000) == 0x1000)
#define KEY_LT_LAYER(kc)    ((uint8_t) (((kc) >> 8) & 0x0F))

/// Special Keys

#define KEY_TRANSPARENT     ((keycode_t) 0x2000) // falls through to the layer below
#define KEY_BOOTSEL         ((keycode_t) 0x2001) // reboot into BOOTSEL once held BOOTSEL_HOLD_MS

// Ctrl + Shift + Alt + GUI
#define KEY_HYPER           MODS(KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT | \
                                 KEYBOARD_MODIFIER_LEFTALT | KEYBOARD_MODIFIER_LEFTGUI, HID_KEY_NONE)

/// Cleanup naming scheme

#define _______             KEY_TRANSPARENT
#define XXXXXXX             HID_KEY_NONE

extern keycode_t const keymap[NUM_LAYERS][NUM_KEYS];

// Process all keymap positions, bit n set while position n is held
void keyboard_task(uint64_t keys, uint32_t now_ms);

// Next report for the host, false if none waiting
bool keyboard_next_report(hid_keyboard_report_t* report);

#endif /* KEYBOARD_H_ */
