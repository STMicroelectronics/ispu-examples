#include "peripherals.h"
#include "reg_map.h"

static volatile uint32_t int_status;

void __attribute__ ((signal)) algo_00_init(void)
{
	// TODO: initialize algorithm
}

void __attribute__ ((signal)) algo_00(void)
{
	// TODO: read and process sensor data

	// interrupt generation (set bit 0 for algo 0, bit 1 for algo 1, etc.)
	int_status = int_status | 0x1;
}

// For more algorithms implement the corresponding functions: algo_01_init and
// algo_01 for algo 1, algo_02_init and algo_02 for algo 2, etc.

int main(void)
{
	// set boot done flag
	uint8_t status = cast_uint8_t(STRED_STATUS);
	status = status | 0x04;
	cast_uint8_t(STRED_STATUS) = status;

	while (1) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0;
		cast_uint8_t(STRED_INT_STATUS) = 0x00;
		cast_uint8_t(STRED_INT_PAD) = 0x00;

		// get all the algorithms to run in this time slot
		cast_uint32_t(STRED_CALL_EN) = cast_uint32_t(STRED_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(STRED_CALL_EN)) {}

		// set status registers and generate interrupts
		cast_uint32_t(STRED_INT_STATUS) = int_status;
		cast_uint8_t(STRED_INT_PAD) = ((int_status & cast_uint32_t(STRED_INT1_CTRL) > 0) << 0) |
			((int_status & cast_uint32_t(STRED_INT2_CTRL) > 0) << 1);
	}
}

