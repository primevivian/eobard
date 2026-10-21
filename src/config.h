// Eobard firmware configuration
// Board specific settings live here and in matrix.c, set for the KMK diode prototype

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>

/// Side

// Set by CMake, 1 on eobard_left and 0 on eobard_right
#ifndef EOBARD_LEFT
#error EOBARD_LEFT must be defined (1 for the left half, 0 for the right half)
#endif

/// Key Matrix and Diodes

#define MATRIX_ROWS       4
#define MATRIX_COLS       5
#define MATRIX_ROW_PINS   { 2, 3, 4, 5 }
#define MATRIX_COL_PINS   { 10, 11, 12, 13, 14 }

// ROW2COL diodes, columns get driven low and rows read with pull-ups

// Matrix must be stable this long before a change counts
#define DEBOUNCE_MS       5

// Both halves together, 4 rows x 10 columns like the KMK keymap
#define NUM_KEYS          (MATRIX_ROWS * MATRIX_COLS * 2)

/// Split UART

// Cable runs GP8 to GP8 and GP9 to GP9, each half sends where the other listens
#if EOBARD_LEFT
#define SPLIT_TX_PIN      8
#define SPLIT_RX_PIN      9
#else
#define SPLIT_TX_PIN      9
#define SPLIT_RX_PIN      8
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
