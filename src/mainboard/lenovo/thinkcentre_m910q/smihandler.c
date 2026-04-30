/* SPDX-License-Identifier: GPL-2.0-only */

#include <acpi/acpi.h>
#include <cpu/x86/smm.h>
#include <intelblocks/pcr.h>
#include <soc/pcr_ids.h>

/*
 * Pulse the chassis power LED while suspended. The PWM block (set up at
 * ramstage to 0.5 Hz, 0% duty = LED solid on via PW_LED# / GPP_D1 NF4) is
 * reprogrammed here to ~50% duty so the LED visibly blinks in S3/S4.
 */
void mainboard_smi_sleep(u8 slp_typ)
{
	switch (slp_typ) {
	case ACPI_S3:
	case ACPI_S4:
		pcr_rmw32(PID_GPIOCOM1, 0x204, 0xffffff00, 0x7f);
		pcr_or32(PID_GPIOCOM1, 0x204, (1 << 30));
		break;
	default:
		break;
	}
}
