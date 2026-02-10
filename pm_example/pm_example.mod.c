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



static const char ____versions[]
__used __section("__versions") =
	"\x24\x00\x00\x00\x5d\x88\x77\x4a"
	"platform_driver_unregister\0\0"
	"\x18\x00\x00\x00\x21\xb7\x93\xa1"
	"devm_kmalloc\0\0\0\0"
	"\x20\x00\x00\x00\xa4\x9a\x07\x37"
	"devm_clk_get_optional\0\0\0"
	"\x24\x00\x00\x00\x87\xa4\x39\x18"
	"devm_regulator_get_optional\0"
	"\x1c\x00\x00\x00\x8b\x51\x34\xc6"
	"regulator_enable\0\0\0\0"
	"\x14\x00\x00\x00\x71\x73\x9a\x7c"
	"clk_prepare\0"
	"\x14\x00\x00\x00\xa6\x88\x55\x81"
	"clk_enable\0\0"
	"\x1c\x00\x00\x00\x4e\x62\x3e\x98"
	"pm_runtime_enable\0\0\0"
	"\x20\x00\x00\x00\xc1\x88\x2e\xf7"
	"__pm_runtime_set_status\0"
	"\x24\x00\x00\x00\xba\x9f\x50\x23"
	"__platform_driver_register\0\0"
	"\x28\x00\x00\x00\x25\x67\xf0\xf6"
	"platform_device_register_full\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x14\x00\x00\x00\x62\xcf\x79\x60"
	"_dev_info\0\0\0"
	"\x14\x00\x00\x00\x9d\xd9\xe6\xb6"
	"clk_disable\0"
	"\x18\x00\x00\x00\x0a\xe7\x77\xb0"
	"clk_unprepare\0\0\0"
	"\x1c\x00\x00\x00\x4c\xf9\x5e\x07"
	"regulator_disable\0\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x20\x00\x00\x00\xef\x5a\xb9\x21"
	"__pm_runtime_disable\0\0\0\0"
	"\x24\x00\x00\x00\x32\x86\xa0\x32"
	"platform_device_unregister\0\0"
	"\x18\x00\x00\x00\xeb\x7b\x33\xe1"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "DA8C6BA68D499AE97101656");
