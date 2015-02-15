/*
 * ac100.c - MFD core driver for X-Powers' AC100 Audio Codec IC
 *
 * The AC100 is a highly integrated audio codec and RTC subsystem designed
 * for mobile applications. It has 3 I2S/PCM interfaces, a 2 channel DAC,
 * a 2 channel ADC with 5 inputs and a builtin mixer. The RTC subsystem has
 * 3 clock outputs.
 *
 * The audio codec and RTC parts are completely separate, sharing only the
 * host interface for access to its registers.
 *
 * Author: Chen-Yu Tsai <wens@csie.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/mfd/core.h>
#include <linux/mfd/ac100.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/sunxi-rsb.h>

static const struct regmap_range ac100_writeable_ranges[] = {
	regmap_reg_range(AC100_CLK32K_ANALOG_CTRL, AC100_CLK32K_OUT_CTRL3),
	regmap_reg_range(AC100_RTC_RST, AC100_RTC_UPD),
	regmap_reg_range(AC100_ALM_INT_ENA, AC100_ALM_INT_STA),
	regmap_reg_range(AC100_ALM_SEC, AC100_RTC_GP(15)),
};

static const struct regmap_range ac100_volatile_ranges[] = {
	regmap_reg_range(AC100_RTC_RST, AC100_RTC_RST),
	regmap_reg_range(AC100_RTC_SEC, AC100_ALM_INT_STA),
	regmap_reg_range(AC100_ALM_SEC, AC100_ALM_UPD),
};

static const struct regmap_access_table ac100_writeable_table = {
	.yes_ranges	= ac100_writeable_ranges,
	.n_yes_ranges	= ARRAY_SIZE(ac100_writeable_ranges),
};

static const struct regmap_access_table ac100_volatile_table = {
	.yes_ranges	= ac100_volatile_ranges,
	.n_yes_ranges	= ARRAY_SIZE(ac100_volatile_ranges),
};

static const struct regmap_config ac100_regmap_config = {
	.reg_bits	= 8,
	.val_bits	= 16,
	.wr_table	= &ac100_writeable_table,
	.volatile_table	= &ac100_volatile_table,
	.max_register	= AC100_RTC_GP(15),
	.cache_type	= REGCACHE_RBTREE,
};

static struct mfd_cell ac100_cells[] = {
	{
		.name		= "ac100-codec",
		.of_compatible	= "x-powers,ac100-codec",
	}, {
		.name		= "ac100-rtc",
		.of_compatible	= "x-powers,ac100-rtc",
	},
};

static int ac100_rsb_probe(struct sunxi_rsb_device *rdev)
{
	struct ac100_dev *ac100;
	int ret;

	ac100 = devm_kzalloc(&rdev->dev, sizeof(*ac100), GFP_KERNEL);
	if (!ac100)
		return -ENOMEM;

	ac100->dev = &rdev->dev;
	sunxi_rsb_device_set_drvdata(rdev, ac100);

	ac100->regmap = devm_regmap_init_sunxi_rsb(rdev, &ac100_regmap_config);
	if (IS_ERR(ac100->regmap)) {
		ret = PTR_ERR(ac100->regmap);
		dev_err(ac100->dev, "regmap init failed: %d\n", ret);
		return ret;
	}

	ret = mfd_add_devices(ac100->dev, PLATFORM_DEVID_NONE, ac100_cells,
			      ARRAY_SIZE(ac100_cells), NULL, 0, NULL);

	if (ret) {
		dev_err(ac100->dev, "failed to add MFD devices: %d\n", ret);
		return ret;
	}

	return 0;
}

static int ac100_rsb_remove(struct sunxi_rsb_device *rdev)
{
	struct ac100_dev *ac100 = sunxi_rsb_device_get_drvdata(rdev);

	mfd_remove_devices(ac100->dev);

	return 0;
}

static const struct of_device_id ac100_of_match[] = {
	{ .compatible = "x-powers,ac100" },
	{ },
};
MODULE_DEVICE_TABLE(of, ac100_of_match);

static struct sunxi_rsb_driver ac100_rsb_driver = {
	.driver = {
		.name	= "ac100",
		.of_match_table	= of_match_ptr(ac100_of_match),
	},
	.probe	= ac100_rsb_probe,
	.remove	= ac100_rsb_remove,
};
module_sunxi_rsb_driver(ac100_rsb_driver);

MODULE_DESCRIPTION("Audio codec MFD core driver for AC100");
MODULE_AUTHOR("Chen-Yu Tsai <wens@csie.org>");
MODULE_LICENSE("GPL");
