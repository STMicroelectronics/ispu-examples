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

#include <stdint.h>
#include <stdbool.h>
#include "motion_wt.h"

#define ACC_SENS 0.000488f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

void __attribute__ ((signal)) algo_00_init(void)
{
	MWT_conf_t conf;

	MotionWT_initialize();

	MotionWT_get_configuration(&conf);

	conf.wrist_up_en = 1;
	conf.wrist_down_en = 1;

	conf.wrist_up_trigger_ths = 0.2f;
	conf.wrist_up_trigger_t = 7; // @ 26 Hz
	conf.wrist_up_tilt_x_ths = 0.5f;
	conf.wrist_up_tilt_y_ths = -0.175f;
	conf.wrist_up_tilt_t = 4; // @ 26 Hz

	conf.wrist_down_trigger_ths = -0.2f;
	conf.wrist_down_trigger_t = 14; // @ 26 Hz
	conf.wrist_down_tilt_x_ths = 0.7071f;
	conf.wrist_down_tilt_y_ths = 0.342f;
	conf.wrist_down_tilt_t = 4; // @ 26 Hz

	MotionWT_set_configuration(&conf);
}

void __attribute__ ((signal)) algo_00(void)
{
	MWT_input_t in;
	MWT_output_t out = { 0 };

	in.acc[0] = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.acc[1] = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.acc[2] = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;

	MotionWT_update(&out, &in);

	cast_float(ISPU_DOUT_00) = in.acc[0];
	cast_float(ISPU_DOUT_02) = in.acc[1];
	cast_float(ISPU_DOUT_04) = in.acc[2];
	cast_uint8_t(ISPU_DOUT_06) = out.wrist_up;
	cast_uint8_t(ISPU_DOUT_06_H) = out.wrist_down;

	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot done flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x04u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x01u;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint32_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		// get all the algorithms to run in this time slot
		cast_uint32_t(ISPU_CALL_EN) = cast_uint32_t(ISPU_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN) != 0u) {
		}

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint32_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

