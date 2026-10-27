// Eobard firmware configuration
// Board specific settings live here and in matrix.c, set for the diodeless rev 1.0 PCB

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>

/// Side

// Set by CMake, 1 on eobard_left and 0 on eobard_right
#ifndef EOBARD_LEFT
#error EOBARD_LEFT must be defined (1 for the left half, 0 for the right half)
#endif

/// Direct Wired Switches

// No diodes or matrix, each switch shorts its own pin to GND and gets read with a pull-up
// Pins of SW1 to SW17 from the KiCad schematic
#define NUM_SWITCHES      17
#define SWITCH_PINS       { 6, 5, 4, 3, 2, 11, 10, 9, 8, 7, 15, 14, 13, 12, 20, 19, 18 }

// Switches must be stable this long before a change counts
#define DEBOUNCE_MS       5

/// Keymap Layout

// Switches fill the keymap five to a row from the outer column,
// SW16 (outer thumb) and SW17 (inner thumb) take the first two slots of the thumb row
#define KEYMAP_ROWS       4
#define KEYMAP_COLS       5    // per half

// Both halves together, 4 rows x 10 columns like the KMK keymap
#define NUM_KEYS          (KEYMAP_ROWS * KEYMAP_COLS * 2)

/// Split UART

// Both jacks wire GP17 to R1 and GP16 to R2, cable joins them like to like,
// so the right half swaps pins to send where the other listens
#if EOBARD_LEFT
#define SPLIT_TX_PIN      17
#define SPLIT_RX_PIN      16
#else
#define SPLIT_TX_PIN      16
#define SPLIT_RX_PIN      17
#endif

#define SPLIT_BAUD                115200
#define SPLIT_KEEPALIVE_MS        10   // resend keys at least this often
#define SPLIT_TIMEOUT_MS          100  // other half is gone after this long

/// Layer Taps

// Tap under this, hold over it (KMK default)
#define TAPPING_TERM_MS   300

/// Debug

// Printed over UART on GP0/GP1 at 115200 baud
#define DEBUG_ENABLED     1

#define DEBUG_PRINT(...) do { if (DEBUG_ENABLED) printf(__VA_ARGS__); } while (0)

#endif /* CONFIG_H_ */
