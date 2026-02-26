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
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x1004e946, "kmalloc_caches" },
	{ 0xbf55f104, "kmalloc_trace" },
	{ 0xe97b122c, "input_register_handle" },
	{ 0xf402c8b7, "input_open_device" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x7cc8df1c, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x37ab9dc6, "gpiod_set_value" },
	{ 0x381657e7, "devm_kmalloc" },
	{ 0x172e5bed, "devm_gpiod_get" },
	{ 0xde80cd09, "ioremap" },
	{ 0x401d3ae8, "spi_setup" },
	{ 0xf9a482f9, "msleep" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x81daace6, "cdev_init" },
	{ 0x6a7b86fa, "cdev_add" },
	{ 0xa4bf0f83, "class_create" },
	{ 0x4ed0e44, "device_create" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0x30c3eb54, "input_register_handler" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xedc03953, "iounmap" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xb9e041b9, "input_unregister_handler" },
	{ 0x6445ca15, "device_destroy" },
	{ 0xeea0e0d, "class_destroy" },
	{ 0x67d01ca4, "cdev_del" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x12dbcbb6, "__spi_register_driver" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x2a3c0d77, "input_close_device" },
	{ 0x1918c1d7, "input_unregister_handle" },
	{ 0x37a0cba, "kfree" },
	{ 0xa78af5f3, "ioread32" },
	{ 0x4a453f53, "iowrite32" },
	{ 0x23ffb293, "driver_unregister" },
	{ 0x9166fada, "strncpy" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x73776b79, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cilitek,ili9225");
MODULE_ALIAS("of:N*T*Cilitek,ili9225C*");

MODULE_INFO(srcversion, "E424F03FF8C14B444E182FE");
