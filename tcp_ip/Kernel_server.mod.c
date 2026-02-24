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



static const char ____versions[]
__used __section("__versions") =
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x74\x39\x9f\x68"
	"kthread_stop\0\0\0\0"
	"\x14\x00\x00\x00\xfe\x8c\x6a\x24"
	"init_net\0\0\0\0"
	"\x1c\x00\x00\x00\x52\x90\xa3\xa3"
	"sock_create_kern\0\0\0\0"
	"\x14\x00\x00\x00\x22\xbd\x7e\x3f"
	"kernel_bind\0"
	"\x18\x00\x00\x00\x40\x9b\xca\xd1"
	"kernel_listen\0\0\0"
	"\x18\x00\x00\x00\x98\x51\x63\x3e"
	"kernel_accept\0\0\0"
	"\x18\x00\x00\x00\x4c\x08\x04\x81"
	"kernel_recvmsg\0\0"
	"\x18\x00\x00\x00\xc7\x23\xcc\xbc"
	"kernel_sendmsg\0\0"
	"\x18\x00\x00\x00\xc4\xc1\x53\x0b"
	"sock_release\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x20\x00\x00\x00\xcb\xae\x17\x38"
	"kthread_create_on_node\0\0"
	"\x18\x00\x00\x00\x90\x03\xc3\xec"
	"wake_up_process\0"
	"\x18\x00\x00\x00\x2e\x9f\xe7\xf6"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "4012F8A7E21F9605DB3496F");
