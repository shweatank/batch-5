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
	{ 0x1485de59, "gpiod_set_value" },
	{ 0xd30648e9, "spi_setup" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0xf9a482f9, "msleep" },
	{ 0x14d51ef4, "_dev_info" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cilitek,ili9225");
MODULE_ALIAS("of:N*T*Cilitek,ili9225C*");

MODULE_INFO(srcversion, "9DA2A4AEF0D41EB05EFBF96");
