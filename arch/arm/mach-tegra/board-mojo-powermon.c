/*
 * arch/arm/mach-tegra/board-mojo-powermon.c
 *
 * Copyright (c) 2011-2013, NVIDIA CORPORATION. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/i2c.h>
#include <linux/ina3221.h>

#include "board.h"
#include "board-mojo.h"

enum {
	VDD_DDR_CORE_CPU,
};

static struct ina3221_platform_data power_mon_info[] = {
	[VDD_DDR_CORE_CPU] = {
        .rail_name = {"PMU_MUX_RSENSE", "VDD_5V_CPU", "NOT_CONNECTED"},
        .shunt_resistor= {INT_MAX, 5, INT_MAX},        
		.cont_conf_data = INA3221_CONT_CONFIG_DATA,
		.trig_conf_data = INA3221_TRIG_CONFIG_DATA,
	},
};

enum {
	INA_I2C_ADDR_40,
};

static struct i2c_board_info mojo_i2c1_ina3221_board_info[] = {
	[INA_I2C_ADDR_40] = {
		I2C_BOARD_INFO("ina3221", 0x40),
		.platform_data = &power_mon_info[VDD_DDR_CORE_CPU],
		.irq = -1,
	},
};

int __init mojo_pmon_init(void)
{
	pr_info("INA3221: registering device\n");
	i2c_register_board_info(1, mojo_i2c1_ina3221_board_info,
		ARRAY_SIZE(mojo_i2c1_ina3221_board_info));

	return 0;
}
