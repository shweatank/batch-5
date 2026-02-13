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
	{ 0x14d51ef4, "_dev_info" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0x166f4b12, "regulator_disable" },
	{ 0x2587b615, "__pm_runtime_disable" },
	{ 0x23e91bcc, "platform_device_unregister" },
	{ 0x61fd46a9, "platform_driver_unregister" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x815588a6, "clk_enable" },
	{ 0xc9cf63fe, "regulator_enable" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0xef54673c, "devm_clk_get_optional" },
	{ 0xe7273eb9, "devm_regulator_get_optional" },
	{ 0x4dcc3608, "pm_runtime_enable" },
	{ 0x437d0342, "__pm_runtime_set_status" },
	{ 0xfa474811, "__platform_driver_register" },
	{ 0xdcb764ad, "memset" },
	{ 0xdcbfa77, "platform_device_register_full" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "00EEE0334ADAA5C42E5571F");
