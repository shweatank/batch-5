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
	{ 0x7c1dcb20, "i2c_register_driver" },
	{ 0x92997ed8, "_printk" },
	{ 0x44b05684, "i2c_transfer_buffer_flags" },
	{ 0xf9a482f9, "msleep" },
	{ 0xd8ff692a, "i2c_del_driver" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Clcd_ldd");
MODULE_ALIAS("of:N*T*Clcd_lddC*");

MODULE_INFO(srcversion, "7007AA315B90FB40B003D30");
