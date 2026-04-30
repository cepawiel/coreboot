/* SPDX-License-Identifier: GPL-2.0-only */

#include <bootblock_common.h>
#include <device/pnp_ops.h>
#include <soc/gpio.h>
#include <superio/nuvoton/common/nuvoton.h>
#include <superio/nuvoton/nct6687d/nct6687d.h>

#include "gpio.h"

#define SERIAL_DEV PNP_DEV(0x2e, NCT6687D_SP2)

static void early_sio_global_config(void)
{
	nuvoton_pnp_enter_conf_state(SERIAL_DEV);
	pnp_write_config(SERIAL_DEV, 0x12, 0x00);
	pnp_write_config(SERIAL_DEV, 0x15, 0xf0);
	pnp_write_config(SERIAL_DEV, 0x1a, 0x03);
	pnp_write_config(SERIAL_DEV, 0x1b, 0xf0);
	pnp_write_config(SERIAL_DEV, 0x1c, 0x00);
	pnp_write_config(SERIAL_DEV, 0x1d, 0x00);
	pnp_write_config(SERIAL_DEV, 0x1e, 0xf0);
	pnp_write_config(SERIAL_DEV, 0x1f, 0xf0);
	pnp_write_config(SERIAL_DEV, 0x26, 0x00);
	pnp_write_config(SERIAL_DEV, 0x27, 0x3e);
	pnp_write_config(SERIAL_DEV, 0x28, 0x00);
	pnp_write_config(SERIAL_DEV, 0x29, 0xed);
	pnp_write_config(SERIAL_DEV, 0x2a, 0xcf);
	pnp_write_config(SERIAL_DEV, 0x2b, 0x00);
	pnp_write_config(SERIAL_DEV, 0x2c, 0x00);
	pnp_write_config(SERIAL_DEV, 0x2d, 0x00);
	pnp_write_config(SERIAL_DEV, 0x2e, 0x00);
	pnp_write_config(SERIAL_DEV, 0x2f, 0x10);
	nuvoton_pnp_exit_conf_state(SERIAL_DEV);
}

void bootblock_mainboard_early_init(void)
{
	early_sio_global_config();
	nuvoton_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
}

void bootblock_mainboard_init(void)
{
	/* Configure all pads here - FSP-M (called from romstage) brings up the
	 * PCH PCIe links and needs CLKREQ pins in NF1 mode AND any board-side
	 * power-enable GPIOs asserted before training. If we wait until ramstage
	 * to apply the pad table the M.2 E-key (rp1) link never trains and FSP
	 * silently hides the root port. */
	gpio_configure_pads(gpio_table, ARRAY_SIZE(gpio_table));
}
