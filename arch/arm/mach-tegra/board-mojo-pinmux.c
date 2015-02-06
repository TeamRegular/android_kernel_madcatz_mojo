/*
 * arch/arm/mach-tegra/board-mojo-pinmux.c
 *
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <mach/pinmux.h>
#include <mach/gpio-tegra.h>
#include "board.h"
#include "board-mojo.h"
#include "devices.h"
#include "gpio-names.h"

#include <mach/pinmux-t11.h>

static __initdata struct tegra_drive_pingroup_config mojo_drive_pinmux[] = {
	/* DEFAULT_DRIVE(<pin_group>), */
	/* SDMMC1 */
	SET_DRIVE(SDIO1, ENABLE, DISABLE, DIV_1, 36, 20, SLOW, SLOW),

	/* SDMMC3 */
	SET_DRIVE(SDIO3, ENABLE, DISABLE, DIV_1, 22, 36, FASTEST, FASTEST),

	/* SDMMC4 */
	SET_DRIVE_WITH_TYPE(GMA, ENABLE, DISABLE, DIV_1, 2, 2, FASTEST,
								FASTEST, 1),
};

#include "board-mojo-pinmux-t11x.h"

/* THIS IS FOR EXPERIMENTAL OR WORKAROUND PURPOSES. ANYTHING INSIDE THIS TABLE
 * SHOULD BE CONSIDERED TO BE PUSHED TO PINMUX SPREADSHEET FOR CONSISTENCY
 */
static __initdata struct tegra_pingroup_config manual_config_pinmux[] = {

	/* ULPI SFIOs are not supposed to be supported.
	 * This setting is only for Mojo. */
	DEFAULT_PINMUX(ULPI_DATA0,    ULPI,        NORMAL,    NORMAL,   INPUT),
	DEFAULT_PINMUX(ULPI_DATA1,    ULPI,        NORMAL,    NORMAL,   INPUT),
	DEFAULT_PINMUX(ULPI_DATA5,    ULPI,        NORMAL,    NORMAL,   INPUT),
	DEFAULT_PINMUX(ULPI_DATA6,    ULPI,        NORMAL,    NORMAL,   INPUT),
	DEFAULT_PINMUX(ULPI_DATA7,    ULPI,        NORMAL,    NORMAL,   INPUT),
};

static void __init mojo_gpio_init_configure(void)
{
	int len;
	int i;
	struct gpio_init_pin_info *pins_info;

	len = ARRAY_SIZE(init_gpio_mode_mojo_common);
	pins_info = init_gpio_mode_mojo_common;

	for (i = 0; i < len; ++i) {
		tegra_gpio_init_configure(pins_info->gpio_nr,
			pins_info->is_input, pins_info->value);
		pins_info++;
	}
}

#ifdef CONFIG_PM_SLEEP
/* pinmux settings during low power mode for power saving purpose */
static struct tegra_pingroup_config mojo_sleep_pinmux[] = {
	/* VDDIO_HV */
	GPIO_PINMUX(SPDIF_IN, NORMAL, NORMAL, OUTPUT, DISABLE),
	/* VDDIO_GMI*/
	DEFAULT_PINMUX(GMI_AD7,       SPI4,        NORMAL,   NORMAL,   INPUT),
	GPIO_PINMUX(GMI_CS2_N,  NORMAL,   NORMAL,   INPUT,   DISABLE),
	GPIO_PINMUX(GMI_CS7_N,  NORMAL,   NORMAL,   INPUT,   DISABLE),
};
#endif

int __init mojo_pinmux_init(void)
{
	mojo_gpio_init_configure();

	tegra_pinmux_config_table(mojo_pinmux_common,
					ARRAY_SIZE(mojo_pinmux_common));
	tegra_drive_pinmux_config_table(mojo_drive_pinmux,
					ARRAY_SIZE(mojo_drive_pinmux));
	tegra_pinmux_config_table(unused_pins_lowpower,
		ARRAY_SIZE(unused_pins_lowpower));
	tegra_pinmux_config_table(manual_config_pinmux,
		ARRAY_SIZE(manual_config_pinmux));
#ifdef CONFIG_PM_SLEEP
	tegra11x_set_sleep_pinmux(mojo_sleep_pinmux,
		ARRAY_SIZE(mojo_sleep_pinmux));
#endif

	return 0;
}
