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
	{ 0xfa474811, "__platform_driver_register" },
	{ 0x61fd46a9, "platform_driver_unregister" },
	{ 0xdf484963, "device_destroy" },
	{ 0x6775d5d3, "class_destroy" },
	{ 0x27271c6b, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xb6e6d99d, "clk_disable" },
	{ 0xb077e70a, "clk_unprepare" },
	{ 0x92997ed8, "_printk" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0xef7fa408, "platform_get_resource" },
	{ 0xcae3b64b, "devm_ioremap_resource" },
	{ 0xe856a90, "devm_clk_get" },
	{ 0x7c9a7371, "clk_prepare" },
	{ 0x556e4390, "clk_get_rate" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xa01f13a6, "cdev_init" },
	{ 0x3a6d85d3, "cdev_add" },
	{ 0x59c02473, "class_create" },
	{ 0xb63fdcdb, "device_create" },
	{ 0x815588a6, "clk_enable" },
	{ 0xdcb764ad, "memset" },
	{ 0x6cbbfc54, "__arch_copy_to_user" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmy,uart");
MODULE_ALIAS("of:N*T*Cmy,uartC*");

MODULE_INFO(srcversion, "5D6A8E801E6018D62CC51C7");
