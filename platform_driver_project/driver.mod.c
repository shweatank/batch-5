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
	{ 0x381657e7, "devm_kmalloc" },
	{ 0x76dd3543, "platform_get_resource" },
	{ 0x5378601d, "devm_ioremap_resource" },
	{ 0x3ad096e2, "devm_gpiod_get_optional" },
	{ 0x16e75c5d, "gpiod_set_value_cansleep" },
	{ 0xdac3f607, "device_create_file" },
	{ 0x5b2a79f4, "platform_get_irq_optional" },
	{ 0x72178972, "devm_request_threaded_irq" },
	{ 0xceb19f3e, "_dev_err" },
	{ 0xca6aceef, "device_remove_file" },
	{ 0xf4f31022, "platform_driver_unregister" },
	{ 0x124bad4d, "kstrtobool" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x9a7b4bd7, "__platform_driver_register" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xe783e261, "sysfs_emit" },
	{ 0x4985390f, "_dev_info" },
	{ 0x73776b79, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cdemo,my-platform-device");
MODULE_ALIAS("of:N*T*Cdemo,my-platform-deviceC*");

MODULE_INFO(srcversion, "7A9B9988AD5955EFB81CF84");
