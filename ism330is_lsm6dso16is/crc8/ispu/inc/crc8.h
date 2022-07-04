#ifndef _CRC8_H_
#define _CRC8_H_

#include <stdint.h>

void crc8_init(void);
uint8_t crc8_run(uint8_t *in, uint16_t len);

#endif

