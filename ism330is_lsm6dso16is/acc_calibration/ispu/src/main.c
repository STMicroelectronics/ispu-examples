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

#include <stddef.h>
#include "motion_ac.h"

#define ACC_SENS 0.000244f

static volatile uint32_t int_status;

static float dtime;
static uint32_t n;
static MAC_output_t data_out;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	dtime = *((float *)&dt) * 2000.0f;

	MotionAC_Initialize(1);

	MAC_knobs_t knobs;
	MotionAC_GetKnobs(&knobs);
	knobs.Sample_ms = (uint32_t)dtime;
	MotionAC_SetKnobs(&knobs);

	n = 0;
	MotionAC_GetCalParams(&data_out);
}

void __attribute__ ((signal)) algo_00(void)
{
	MAC_input_t data_in;
	uint8_t cal;

	data_in.Acc[0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	data_in.Acc[1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	data_in.Acc[2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	data_in.TimeStamp = (int)(n * dtime);
	n++;

	MotionAC_Update(&data_in, &cal);

	if (cal)
		MotionAC_GetCalParams(&data_out);

	cast_float(ISPU_DOUT_00) = (data_in.Acc[0] - data_out.AccBias[0]) * data_out.SF_Matrix[0][0];
	cast_float(ISPU_DOUT_02) = (data_in.Acc[1] - data_out.AccBias[1]) * data_out.SF_Matrix[1][1];
	cast_float(ISPU_DOUT_04) = (data_in.Acc[2] - data_out.AccBias[2]) * data_out.SF_Matrix[2][2];
	cast_float(ISPU_DOUT_06) = data_out.AccBias[0];
	cast_float(ISPU_DOUT_08) = data_out.AccBias[1];
	cast_float(ISPU_DOUT_10) = data_out.AccBias[2];
	cast_float(ISPU_DOUT_12) = data_out.SF_Matrix[0][0];
	cast_float(ISPU_DOUT_14) = data_out.SF_Matrix[1][1];
	cast_float(ISPU_DOUT_16) = data_out.SF_Matrix[2][2];
	cast_uint8_t(ISPU_DOUT_18) = (uint8_t)data_out.CalQuality;

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

