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
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xfe990052, "gpio_free" },
	{ 0x122c3a7e, "_printk" },
	{ 0x403f9529, "gpio_request_one" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C2D64D95B541027F6A800E3");
