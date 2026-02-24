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
	{ 0x82ee90dc, "timer_delete_sync" },
	{ 0x1485de59, "gpiod_set_value" },
	{ 0x14d51ef4, "_dev_info" },
	{ 0xfa474811, "__platform_driver_register" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x88d4c348, "_dev_err" },
	{ 0x61fd46a9, "platform_driver_unregister" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ctechdhaba,gpio-led");
MODULE_ALIAS("of:N*T*Ctechdhaba,gpio-ledC*");

MODULE_INFO(srcversion, "7A97B3AC2E773D0A7A62B45");
