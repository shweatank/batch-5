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
	{ 0xd45b3bc6, "kthread_stop" },
	{ 0x102fe6de, "hrtimer_cancel" },
	{ 0x1485de59, "gpiod_set_value" },
	{ 0x14d51ef4, "_dev_info" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x3a2a0265, "gpiod_get_value" },
	{ 0x34db050b, "_raw_spin_lock_irqsave" },
	{ 0xc0b7c197, "hrtimer_start_range_ns" },
	{ 0xd35cce70, "_raw_spin_unlock_irqrestore" },
	{ 0xf9a482f9, "msleep" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0xea82d349, "hrtimer_init" },
	{ 0xcb661d89, "kthread_create_on_node" },
	{ 0xb2cf531a, "wake_up_process" },
	{ 0x777fcc6b, "dev_err_probe" },
	{ 0x61fd46a9, "platform_driver_unregister" },
	{ 0x135bb7ec, "hrtimer_forward" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ctechdhaba,td-gpio-blink");
MODULE_ALIAS("of:N*T*Ctechdhaba,td-gpio-blinkC*");

MODULE_INFO(srcversion, "C356C15F69D59479C95FBDC");
