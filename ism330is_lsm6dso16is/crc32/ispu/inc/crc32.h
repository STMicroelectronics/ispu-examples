#ifndef _CRC32_H_
#define _CRC32_H_

#include <stdint.h>

void crc32_init(void);
uint32_t crc32_run(uint8_t *in, uint16_t len);

#endif

