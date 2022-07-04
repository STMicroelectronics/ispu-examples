#include <stdint.h>
#include "crc8.h"

#define CRC8_POLY 0x07

static uint8_t crc8_table[256];

void crc8_init(void)
{
	uint8_t c;

	for (uint16_t n = 0; n < 256; n++) {
		c = (uint8_t)n;
		for (uint8_t k = 8; k > 0; k--) {
			if (c & 0x80)
				c = (c << 1) ^ CRC8_POLY;
			else
				c = c << 1;
		}
		crc8_table[n] = c;
	}
}

uint8_t crc8_run(uint8_t *in, uint16_t len)
{
	uint8_t c = 0x00;

	for (uint16_t n = 0; n < len; n++)
		c = crc8_table[c ^ in[n]];

	return c ^ 0x00;
}

