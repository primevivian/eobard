// Link between both halves over PIO UART

#ifndef SPLIT_H_
#define SPLIT_H_

#include <stdint.h>

void split_init(void);

// Send this half's keys, get the other half's back (all released if the link drops)
uint32_t split_task(uint32_t local_keys, uint32_t now_ms);

#endif /* SPLIT_H_ */
