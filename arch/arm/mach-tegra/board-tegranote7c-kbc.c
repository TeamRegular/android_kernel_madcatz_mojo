/*
 * Copyright (c) 2014, NVIDIA CORPORATION.  All rights reserved.
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

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <mach/io.h>
#include <linux/io.h>
#include <mach/iomap.h>
#include <mach/kbc.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/mfd/palmas.h>
#include "wakeups-t11x.h"

#include "tegra-board-id.h"
#include "board.h"
#include "board-tegranote7c.h"
#include "devices.h"

#define GPIO_KEY(_id, _gpio, _iswake)           \
	{                                       \
		.code = _id,                    \
		.gpio = TEGRA_GPIO_##_gpio,     \
		.active_low = 1,                \
		.desc = #_id,                   \
		.type = EV_KEY,                 \
		.wakeup = _iswake,              \
		.debounce_interval = 30,        \
	}

#define GPIO_SW(_id, _gpio, _active_low, _iswake)   \
	{                                           \
		.code = _id,                        \
		.gpio = TEGRA_GPIO_##_gpio,         \
		.irq = -1,                          \
		.type = EV_SW,                      \
		.desc = #_id,                       \
		.active_low = _active_low,          \
		.wakeup = _iswake,                  \
		.debounce_interval = 0,             \
	}

/* SW_TABLET_MODE switch is used for stylus pen detection
 * when SW_TABLET_MODE = 1, pen is inserted
 */
static struct gpio_keys_button tegranote7c_p1988_keys[] = {
	[0] = GPIO_KEY(KEY_POWER, PQ0, 1),
	[1] = GPIO_KEY(KEY_VOLUMEUP, PR2, 1),
	[2] = GPIO_KEY(KEY_VOLUMEDOWN, PQ2, 1),
	[3] = GPIO_SW(SW_LID, PC7, 1, 1),
	[4] = GPIO_SW(SW_TABLET_MODE, PO5, 1, 1),
};

static int tegranote7c_wakeup_key(void)
{
	int wakeup_key;
	u64 status = readl(IO_ADDRESS(TEGRA_PMC_BASE) + PMC_WAKE_STATUS)
		| (u64)readl(IO_ADDRESS(TEGRA_PMC_BASE)
		+ PMC_WAKE2_STATUS) << 32;
	if (status & ((u64)1 << TEGRA_WAKE_GPIO_PQ0))
		wakeup_key = KEY_POWER;
	else if (status & ((u64)1 << TEGRA_WAKE_GPIO_PC7))
		wakeup_key = SW_LID;
	else if (status & ((u64)1 << TEGRA_WAKE_GPIO_PO5))
		wakeup_key = SW_TABLET_MODE;
	else
		wakeup_key = KEY_UNKNOWN;

	return wakeup_key;
}

static struct gpio_keys_platform_data tegranote7c_keys_pdata = {
	.buttons	= tegranote7c_p1988_keys,
	.nbuttons	= ARRAY_SIZE(tegranote7c_p1988_keys),
	.wakeup_key	= tegranote7c_wakeup_key,
};

static struct platform_device tegranote7c_keys_device = {
	.name	= "gpio-keys",
	.id	= 0,
	.dev	= {
		.platform_data  = &tegranote7c_keys_pdata,
	},
};

int __init tegranote7c_kbc_init(void)
{
	platform_device_register(&tegranote7c_keys_device);
	return 0;
}

