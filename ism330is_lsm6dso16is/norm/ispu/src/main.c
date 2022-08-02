/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "peripherals.h"
#include "reg_map.h"

#include <math.h>

static volatile uint32_t int_status;

void __attribute__ ((signal)) algo_00_init(void)
{
}

void __attribute__ ((signal)) algo_00(void)
{
	int16_t acc[3];
	float norm;

	acc[0] = cast_sint16_t(ISPU_ARAW_X);
	acc[1] = cast_sint16_t(ISPU_ARAW_Y);
	acc[2] = cast_sint16_t(ISPU_ARAW_Z);

	norm = sqrtf(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);

	cast_sint16_t(ISPU_DOUT_00) = acc[0];
	cast_sint16_t(ISPU_DOUT_01) = acc[1];
	cast_sint16_t(ISPU_DOUT_02) = acc[2];

	cast_float(ISPU_DOUT_03) = norm;

	int_status = int_status | 0x0001;
}

int main(void)
{
	// set boot done flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x04;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x01;

	while (1) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0;
		cast_uint32_t(ISPU_INT_STATUS) = 0;
		cast_uint8_t(ISPU_INT_PIN) = 0;

		// get all the algorithms to run in this time slot
		cast_uint32_t(ISPU_CALL_EN) = cast_uint32_t(ISPU_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN))
			;

		// set status registers and generate interrupts
		cast_uint32_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = (((int_status & cast_uint32_t(ISPU_INT1_CTRL)) > 0) << 0) |
			(((int_status & cast_uint32_t(ISPU_INT2_CTRL)) > 0) << 1);
	}
}

