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

	acc[0] = cast_sint16_t(RL_ADDR_araw_x);
	acc[1] = cast_sint16_t(RL_ADDR_araw_y);
	acc[2] = cast_sint16_t(RL_ADDR_araw_z);

	norm = sqrtf(acc[0] * acc[0] + acc[1] * acc[1] + acc[2] * acc[2]);

	cast_sint16_t(RL_ADDR_DOUT_00) = acc[0];
	cast_sint16_t(RL_ADDR_DOUT_01) = acc[1];
	cast_sint16_t(RL_ADDR_DOUT_02) = acc[2];

	cast_float(RL_ADDR_DOUT_03) = norm;

	int_status = int_status | 0x0001;
}

int main(void)
{
	// set boot done flag
	uint8_t RL_STATUS = cast_uint8_t(RL_ADDR_RL_STATUS);
	RL_STATUS = RL_STATUS | 0x04;
	cast_uint8_t(RL_ADDR_RL_STATUS) = RL_STATUS;

	while (1) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0;
		cast_uint8_t(RL_ADDR_INT_STATUS) = 0x00;
		cast_uint8_t(RL_ADDR_INT_PAD) = 0x00;

		// get all the algorithms to run in this time slot
		cast_uint32_t(RL_ADDR_CALL_EN) = cast_uint32_t(RL_ADDR_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(RL_ADDR_CALL_EN)) {}

		// set status registers and generate interrupts
		cast_uint32_t(RL_ADDR_INT_STATUS) = int_status;
		cast_uint8_t(RL_ADDR_INT_PAD) = ((int_status & cast_uint32_t(RL_ADDR_INT1_CTRL) > 0) << 0) |
			((int_status & cast_uint32_t(RL_ADDR_INT2_CTRL) > 0) << 1);
	}
}

