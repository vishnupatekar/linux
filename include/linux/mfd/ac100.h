/*
 * Functions and registers to access AC100 codec / RTC combo IC.
 *
 * Copyright (C) 2013, Carlo Caione <carlo@caione.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __LINUX_MFD_AC100_H
#define __LINUX_MFD_AC100_H

#include <linux/regmap.h>

struct ac100_dev {
	struct device			*dev;
	struct regmap			*regmap;
};

/* Audio codec related registers */
#define AC100_AUDIO_CHIP_RST		0x00
#define AC100_PLL_CTRL1			0x01
#define AC100_PLL_CTRL2			0x02
#define AC100_SYSCLK_CTRL		0x03
#define AC100_MOD_CLK_ENA		0x04
#define AC100_MOD_RST_CTRL		0x05

/* RTC */
#define AC100_CLK32K_ANALOG_CTRL	0xc0
#define AC100_CLK32K_OUT_CTRL1		0xc1
#define AC100_CLK32K_OUT_CTRL2		0xc2
#define AC100_CLK32K_OUT_CTRL3		0xc3

/* RTC module */
#define AC100_RTC_RST			0xc6
#define AC100_RTC_CTRL			0xc7
#define AC100_RTC_SEC			0xc8 /* second */
#define AC100_RTC_MIN			0xc9 /* minute */
#define AC100_RTC_HOU			0xca /* hour */
#define AC100_RTC_WEE			0xcb /* weekday */
#define AC100_RTC_DAY			0xcc /* day */
#define AC100_RTC_MON			0xcd /* month */
#define AC100_RTC_YEA			0xce /* year */
#define AC100_RTC_UPD			0xcf /* update trigger */

/* RTC Alarm */
#define AC100_ALM_INT_ENA		0xd0
#define	AC100_ALM_INT_STA		0xd1
#define AC100_ALM_SEC			0xd8
#define AC100_ALM_MIN			0xd9
#define AC100_ALM_HOU			0xda
#define AC100_ALM_WEE			0xdb
#define AC100_ALM_DAY			0xdc
#define AC100_ALM_MON			0xdd
#define AC100_ALM_YEA			0xde
#define AC100_ALM_UPD			0xdf

/* RTC general purpose register 0 ~ 15 */
#define AC100_RTC_GP(x)			(0xe0 + (x))

#endif /* __LINUX_MFD_AC100_H */
