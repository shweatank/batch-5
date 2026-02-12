#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x75ef94c, "regulator_disable" },
	{ 0x122c3a7e, "_printk" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x21b95aef, "__pm_runtime_disable" },
	{ 0xa193b721, "devm_kmalloc" },
	{ 0x37079aa4, "devm_clk_get_optional" },
	{ 0x1839a487, "devm_regulator_get_optional" },
	{ 0xc634518b, "regulator_enable" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x815588a6, "clk_enable" },
	{ 0x983e624e, "pm_runtime_enable" },
	{ 0xf72e88c1, "__pm_runtime_set_status" },
	{ 0x23509fba, "__platform_driver_register" },
	{ 0xf6f06725, "platform_device_register_full" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x32a08632, "platform_device_unregister" },
	{ 0x4a77885d, "platform_driver_unregister" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x6079cf62, "_dev_info" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0xe2fd41e5, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "AFB71C9B4C38BFB67F13E80");
