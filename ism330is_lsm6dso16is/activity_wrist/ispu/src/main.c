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

#include "motion_aw.h"

#define ACC_SENS 0.000244f

static volatile uint32_t int_status;
static int64_t timestamp;
static float dtime;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	dtime = *((float *)&dt) * 4000.0f;

	MotionAW_Initialize();
	MotionAW_SetOrientation_Acc("enu");

	timestamp = 0;
}

void __attribute__ ((signal)) algo_00(void)
{
	MAW_input_t in;
	MAW_output_t out;

	in.AccX = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.AccY = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.AccZ = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	timestamp += (int)dtime;

	MotionAW_Update(&in, &out, timestamp);

	cast_float(ISPU_DOUT_00) = in.AccX;
	cast_float(ISPU_DOUT_02) = in.AccY;
	cast_float(ISPU_DOUT_04) = in.AccZ;
	cast_uint8_t(ISPU_DOUT_06) = (uint8_t)out.current_activity;
	cast_uint8_t(ISPU_DOUT_06_B) = (uint8_t)out.confidence;

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

