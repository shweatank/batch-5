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
	{ 0xfd25457e, "_dev_info" },
	{ 0x991b5c57, "i2c_smbus_read_byte_data" },
	{ 0x1b2a7c4e, "_dev_err" },
	{ 0xd8ff692a, "i2c_del_driver" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cmaxim,ds3231");
MODULE_ALIAS("of:N*T*Cmaxim,ds3231C*");

MODULE_INFO(srcversion, "135184530915F11080F207E");
