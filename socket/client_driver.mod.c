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
	{ 0xcb661d89, "kthread_create_on_node" },
	{ 0xb2cf531a, "wake_up_process" },
	{ 0x122c3a7e, "_printk" },
	{ 0xd45b3bc6, "kthread_stop" },
	{ 0xdcb764ad, "memset" },
	{ 0xee616474, "init_net" },
	{ 0x145b5268, "sock_create_kern" },
	{ 0x4ac51a2, "kernel_connect" },
	{ 0xe4929c22, "kernel_sendmsg" },
	{ 0x3613d799, "kernel_recvmsg" },
	{ 0xdf39592e, "sock_release" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F8C5F7DBDF035AFE1A3C84D");
