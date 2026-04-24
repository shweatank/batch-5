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
	{ 0x122c3a7e, "_printk" },
	{ 0x3250fd9c, "devm_gpiod_get" },
	{ 0x9c62fdb0, "gpiod_to_irq" },
	{ 0x3ce80115, "devm_request_threaded_irq" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xa0c562db, "gpio_to_desc" },
	{ 0x7d896ede, "gpiod_direction_output_raw" },
	{ 0xf4d3665c, "gpiod_set_raw_value" },
	{ 0xfe990052, "gpio_free" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Craspi,gpio_button");
MODULE_ALIAS("of:N*T*Craspi,gpio_buttonC*");

MODULE_INFO(srcversion, "136D9E66E3552D3EA95E278");
