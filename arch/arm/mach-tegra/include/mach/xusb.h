/*
 * arch/arm/mach-tegra/include/mach/xusb.h
 *
 * Copyright (c) 2013-2014, NVIDIA CORPORATION.  All rights reserved.
 *
 * Author:
 *	Ajay Gupta <ajayg@nvidia.com>
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
#ifndef _XUSB_H
#define _XUSB_H

/* chip quirks */
#define TEGRA_XUSB_IGNORE_CLK_CHANGE	BIT(0)
/*
 * BIT0 - BIT7 : SS ports
 * BIT8 - BIT15 : USB2 UTMI ports
 * BIT16 - BIT23 : HSIC ports
 * BIT24 - BIT31 : ULPI ports
 */
#define TEGRA_XUSB_SS_P0	(1 << 0)
#define TEGRA_XUSB_SS_P1	(1 << 1)
#define XUSB_SS_PORT_COUNT	(2)
#define XUSB_UTMI_INDEX	(8)
#if defined (CONFIG_ARCH_TEGRA_11x_SOC)
#define XUSB_UTMI_COUNT	(2)
#elif defined (CONFIG_ARCH_TEGRA_12x_SOC)
#define XUSB_UTMI_COUNT	(3)
#define TEGRA_XUSB_USB2_P2	(BIT(XUSB_UTMI_INDEX + 2))
#endif
#define TEGRA_XUSB_USB2_P0	BIT(XUSB_UTMI_INDEX)
#define TEGRA_XUSB_USB2_P1	BIT(XUSB_UTMI_INDEX + 1)
#define TEGRA_XUSB_HSIC_P0	(1 << 16)
#define TEGRA_XUSB_HSIC_P1	(1 << 17)
#define TEGRA_XUSB_ULPI_P0	(1 << 24)
#define TEGRA_XUSB_SS_PORT_MAP_USB2_P0 (0x0)
#define TEGRA_XUSB_SS_PORT_MAP_USB2_P1 (0x1)
#define TEGRA_XUSB_SS0_PORT_MAP	(0xf)
#define TEGRA_XUSB_SS1_PORT_MAP	(0xf0)
#define TEGRA_XUSB_ULPI_PORT_CAP_MASTER	(0x0)
#define TEGRA_XUSB_ULPI_PORT_CAP_PHY	(0x1)

struct tegra_xusb_utmi_config {
	u8 hs_curr_level;
	bool hs_curr_level_override; /* override value from usb_calib0 fuse */
};

struct tegra_xusb_regulator_name {
	u8 *s3p3v;
	u8 *s1p8v;
	u8 *s1p2v;
	u8 *s1p05v;
};

enum vbus_en_type {
	VBUS_FIXED = 0,	/* VBUS enabled by GPIO, without PADCTL OC detection */
	VBUS_FIXED_OC,	/* VBUS enabled by GPIO, with PADCTL detection */
	VBUS_EN_OC,	/* VBUS enabled by XUSB PADCTL */
};

struct usb_vbus_en_oc {
	enum vbus_en_type type;
	void (*set_tristate)(bool on);	/* valid when type is VBUS_EN_OC */
	const char *reg_name;		/* valid when type is VBUS_FIXED[_OC] */
};

struct tegra_xusb_board_data {
	u32	portmap;
	/*
	 * SS0 or SS1 port may be mapped either to USB2_P0 or USB2_P1
	 * ss_portmap[0:3] = SS0 map, ss_portmap[4:7] = SS1 map
	 */
	u8	ss_portmap;
	u8	ulpicap;
	struct tegra_xusb_utmi_config utmi[XUSB_UTMI_COUNT];
	struct usb_vbus_en_oc vbus_en_oc[XUSB_UTMI_COUNT];
	struct tegra_xusb_regulator_name supply;
};

struct tegra_xusb_platform_data {
	struct tegra_xusb_board_data *bdata;
	u32 hs_curr_level_pad0;
	u32 hs_curr_level_pad1;
	u32 hs_iref_cap;
	u32 hs_term_range_adj;
	u32 hs_squelch_level;
	u32 rx_wander;
	u32 rx_eq;
	u32 cdr_cntl;
	u32 dfe_cntl;
	u32 hs_slew;
	u32 ls_rslew;
	u32 hs_disc_lvl;
	/* chip specific */
	unsigned long quirks;
};

#define TEGRA_XUSB_USE_HS_SRC_CLOCK2 BIT(0)

extern struct tegra_xusb_platform_data *tegra_xusb_init(
				struct tegra_xusb_board_data *bdata);
extern void tegra_xusb_register(void);
#endif /* _XUSB_H */
