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
#include <stddef.h>
#include <string.h>
#include "motion_ft.h"

#define ACC_SENS 0.000244f // [g/LSB]

void __attribute__((signal)) algo_00_init(void);
void __attribute__((signal)) algo_00(void);

static volatile uint32_t int_status;

static void *mft;

void __attribute__((signal)) algo_00_init(void)
{
	MFT_conf_t conf = {
		.length = 32u,
		.damp = 0.99f,
		.use_norm = true
	};

	if (mft != NULL) {
		MotionFT_deinitialize(mft);
	}
	mft = MotionFT_initialize(&conf);
}

void __attribute__((signal)) algo_00(void)
{
	MFT_input_t data_in;
	MFT_output_t data_out;
	(void)memset(&data_out, 0, sizeof(data_out));

	// Compute real-time Fourier transform on Z-axis
	data_in.sample = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	MotionFT_update(mft, &data_out, &data_in);

	// Place magnitude of DFT in output registers
	for (uint16_t i = 0u; i < data_out.nfft; i++) {
		cast_float(ISPU_DOUT_00 + ((int16_t)i * 4)) = data_out.mag[i];
	}

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

