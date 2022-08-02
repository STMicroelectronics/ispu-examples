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

#include "iir2.h"

#define ACC_SENS 0.000244f

static volatile uint32_t int_status;

static const float b[3] = { 1.242629257209507e-02,  2.485258514419015e-02, 1.242629257209507e-02 };
static const float a[3] = { 1.000000000000000e+00, -1.660834370919655e+00, 7.105395412080350e-01 };

static struct iir2 filt[3];

void __attribute__ ((signal)) algo_00_init(void)
{
	for (uint8_t i = 0; i < 3; i++)
		iir2_init(&filt[i], b, a);
}

void __attribute__ ((signal)) algo_00(void)
{
	float acc[3], acc_f[3];

	acc[0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	acc[1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	acc[2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;

	for (uint8_t i = 0; i < 3; i++)
		acc_f[i] = iir2_run(&filt[i], acc[i]);

	cast_float(ISPU_DOUT_00) = acc[0];
	cast_float(ISPU_DOUT_02) = acc[1];
	cast_float(ISPU_DOUT_04) = acc[2];
	cast_float(ISPU_DOUT_06) = acc_f[0];
	cast_float(ISPU_DOUT_08) = acc_f[1];
	cast_float(ISPU_DOUT_10) = acc_f[2];

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

