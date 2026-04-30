/* SPDX-License-Identifier: GPL-2.0-only */

#include <device/device.h>
#include <intelblocks/pcr.h>
#include <soc/pcr_ids.h>

#include "gpio.h"

static void init_mainboard(void *chip_info)
{
	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));

	/* Chassis power LED PWM block: 0.5 Hz, 0% TX-high duty.
	 * GPP_D1 in NF4 (PW_LED#) feeds from this block. With 0% duty the
	 * pad sits LOW (LED solid on, since PW_LED# is active-low).
	 * mainboard_smi_sleep() bumps duty to ~50% on S3/S4 entry to blink. */
	pcr_write32(PID_GPIOCOM1, 0x204, (1 << 14));
	pcr_or32(PID_GPIOCOM1, 0x204, (1 << 30));
	pcr_or32(PID_GPIOCOM1, 0x204, (1 << 31));
}

struct chip_operations mainboard_ops = {
	.init = init_mainboard,
};
