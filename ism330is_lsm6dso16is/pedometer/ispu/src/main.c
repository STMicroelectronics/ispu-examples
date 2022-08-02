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

#include "motion_pm.h"

#define ACC_SENS 0.000488f

static volatile uint32_t int_status;

void __attribute__ ((signal)) algo_00_init(void)
{
	MPM_conf_t conf;

	MotionPM_initialize();

	MotionPM_get_configuration(&conf);

	conf.taps_coeff[0] = 0.139526f;
	conf.taps_coeff[1] = 0.083313f;
	conf.taps_coeff[2] = -0.053375f;
	conf.taps_coeff[3] = -0.291504f;
	conf.taps_coeff[4] = -0.192627f;
	conf.dectree_en = 1;
	conf.debounce_steps = 10;
	conf.ths_min = 0.1f;
	conf.coeff_peak_2_peak = 1.381268f;
	conf.max_coeff_up = 0.710269f;
	conf.max_coeff_dn = 0.401648f;

	MotionPM_set_configuration(&conf);
}

void __attribute__ ((signal)) algo_00(void)
{
	MPM_input_t in;
	MPM_output_t out;

	in.acc[0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.acc[1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.acc[2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;

	MotionPM_update(&out, &in);

	cast_float(ISPU_DOUT_00) = in.acc[0];
	cast_float(ISPU_DOUT_02) = in.acc[1];
	cast_float(ISPU_DOUT_04) = in.acc[2];
	cast_uint8_t(ISPU_DOUT_06) = out.steps;

	int_status = int_status | 0x1;
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

