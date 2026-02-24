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
	{ 0xa0c562db, "gpio_to_desc" },
	{ 0xf4d3665c, "gpiod_set_raw_value" },
	{ 0x92997ed8, "_printk" },
	{ 0xd45b3bc6, "kthread_stop" },
	{ 0xfe990052, "gpio_free" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xcf6b2d6, "gpiod_get_raw_value" },
	{ 0xf9a482f9, "msleep" },
	{ 0x47229b5c, "gpio_request" },
	{ 0x7d896ede, "gpiod_direction_output_raw" },
	{ 0x3bdfd684, "gpiod_direction_input" },
	{ 0xcb661d89, "kthread_create_on_node" },
	{ 0xb2cf531a, "wake_up_process" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "EEE53E8D0433914D3E76E0E");
