/* SPDX-License-Identifier: GPL-2.0-only */

#include <assert.h>
#include <string.h>

#include <soc/romstage.h>
#include <spd_bin.h>
#include <types.h>

void mainboard_memory_init_params(FSPM_UPD *mupd)
{
	FSP_M_CONFIG * const mem_cfg = &mupd->FspmConfig;

	mem_cfg->DqPinsInterleaved = true;
	mem_cfg->CaVrefConfig      = 2;

	/* M910q Tiny: 2 DIMM slots, channel A slot 0 = 0x50, channel B slot 0 = 0x52
	 * (matches m720q_m920q layout; m710s SFF used 4 addresses for its 4 slots). */
	struct spd_block blk = {
		.addr_map = {0x50, 0, 0x52, 0},
	};

	get_spd_smbus(&blk);
	mem_cfg->MemorySpdDataLen  = blk.len;
	mem_cfg->MemorySpdPtr00    = (uintptr_t)blk.spd_array[0];
	mem_cfg->MemorySpdPtr01    = (uintptr_t)blk.spd_array[1];
	mem_cfg->MemorySpdPtr10    = (uintptr_t)blk.spd_array[2];
	mem_cfg->MemorySpdPtr11    = (uintptr_t)blk.spd_array[3];
	dump_spd_info(&blk);

	const u16 rcomp_resistors[] = {121, 75, 100};

	assert(sizeof(mem_cfg->RcompResistor) == sizeof(rcomp_resistors));
	memcpy(mem_cfg->RcompResistor, rcomp_resistors, sizeof(rcomp_resistors));

	const u16 rcomp_targets[] = {60, 26, 20, 20, 26};

	assert(sizeof(mem_cfg->RcompTarget) == sizeof(rcomp_targets));
	memcpy(mem_cfg->RcompTarget, rcomp_targets, sizeof(rcomp_targets));
}
