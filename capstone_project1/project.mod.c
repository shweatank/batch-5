#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xf0b59e42, "__spi_register_driver" },
	{ 0x92893115, "driver_unregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x20b64901, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3ce4ca6f, "disable_irq" },
	{ 0xa0c562db, "gpio_to_desc" },
	{ 0xf4d3665c, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0x122c3a7e, "_printk" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xedc03953, "iounmap" },
	{ 0x1485de59, "gpiod_set_value" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x7d896ede, "gpiod_direction_output_raw" },
	{ 0x3bdfd684, "gpiod_direction_input" },
	{ 0xcf6b2d6, "gpiod_get_raw_value" },
	{ 0x656e4a6e, "snprintf" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0xd30648e9, "spi_setup" },
	{ 0xf9a482f9, "msleep" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x9c62fdb0, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0xaf56600a, "arm64_use_ng_mappings" },
	{ 0x40863ba1, "ioremap_prot" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cili9225_display");
MODULE_ALIAS("of:N*T*Cili9225_displayC*");

MODULE_INFO(srcversion, "DAFC82797D5A2DFEEB6C800");
