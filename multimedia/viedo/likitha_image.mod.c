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
	{ 0xdf484963, "device_destroy" },
	{ 0x6775d5d3, "class_destroy" },
	{ 0x27271c6b, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x92893115, "driver_unregister" },
	{ 0xdcb764ad, "memset" },
	{ 0x20b64901, "spi_sync" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x1485de59, "gpiod_set_value" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0xd30648e9, "spi_setup" },
	{ 0xf9a482f9, "msleep" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xa01f13a6, "cdev_init" },
	{ 0x3a6d85d3, "cdev_add" },
	{ 0x59c02473, "class_create" },
	{ 0xb63fdcdb, "device_create" },
	{ 0x14d51ef4, "_dev_info" },
	{ 0x441d0de9, "__kmalloc_large_noprof" },
	{ 0x37a0cba, "kfree" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cilitek,ili9225");
MODULE_ALIAS("of:N*T*Cilitek,ili9225C*");

MODULE_INFO(srcversion, "1290213FCA3F0ED7BB7ABC7");
