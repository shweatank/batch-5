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
	{ 0x7960eb25, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37ab9dc6, "gpiod_set_value" },
	{ 0x381657e7, "devm_kmalloc" },
	{ 0x172e5bed, "devm_gpiod_get" },
	{ 0xcc5a5b0e, "spi_setup" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x475cb629, "v4l2_device_register" },
	{ 0x299ec3de, "video_device_alloc" },
	{ 0x867e696b, "video_device_release" },
	{ 0x534017e0, "__video_register_device" },
	{ 0xf9a482f9, "msleep" },
	{ 0xaf42c85a, "video_devdata" },
	{ 0x9ed12e20, "kmalloc_large" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x37a0cba, "kfree" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x92ae1bd9, "__spi_register_driver" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x7ce125c6, "video_unregister_device" },
	{ 0xc647cdb9, "v4l2_device_unregister" },
	{ 0x122c3a7e, "_printk" },
	{ 0x23ffb293, "driver_unregister" },
	{ 0x73776b79, "module_layout" },
};

MODULE_INFO(depends, "videodev");

MODULE_ALIAS("of:N*T*Cili9225_display");
MODULE_ALIAS("of:N*T*Cili9225_displayC*");

MODULE_INFO(srcversion, "CEFA819024CF47F2781568C");
