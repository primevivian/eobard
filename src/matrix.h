// Switch scanning for one half

#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdbool.h>
#include <stdint.h>

void matrix_init(void);

// Debounced keys of this half, bit n for switch SW(n + 1)
uint32_t matrix_scan(uint32_t now_ms);

// Join both halves into keymap positions, bit n per position
uint64_t matrix_to_positions(uint32_t left_keys, uint32_t right_keys);

#endif /* MATRIX_H_ */
