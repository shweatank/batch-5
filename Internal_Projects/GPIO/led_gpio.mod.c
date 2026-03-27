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
	"\x1c\x00\x00\x00\x55\x05\xd9\x02"
	"gpiod_set_raw_value\0"
	"\x14\x00\x00\x00\x52\x00\x99\xfe"
	"gpio_free\0\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x18\x00\x00\x00\x5c\x9b\x22\x47"
	"gpio_request\0\0\0\0"
	"\x24\x00\x00\x00\x42\x77\xf2\xf3"
	"gpiod_direction_output_raw\0\0"
	"\x20\x00\x00\x00\x6a\xba\xdd\x7b"
	"gpiod_direction_input\0\0\0"
	"\x20\x00\x00\x00\xcb\xae\x17\x38"
	"kthread_create_on_node\0\0"
	"\x18\x00\x00\x00\x90\x03\xc3\xec"
	"wake_up_process\0"
	"\x1c\x00\x00\x00\x6e\x64\xf7\xb3"
	"kthread_should_stop\0"
	"\x1c\x00\x00\x00\x2e\x73\xc7\x54"
	"gpiod_get_raw_value\0"
	"\x10\x00\x00\x00\xf9\x82\xa4\xf9"
	"msleep\0\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x18\x00\x00\x00\x74\x39\x9f\x68"
	"kthread_stop\0\0\0\0"
	"\x18\x00\x00\x00\xb5\x63\xb5\x37"
	"gpio_to_desc\0\0\0\0"
	"\x18\x00\x00\x00\x2e\x9f\xe7\xf6"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E83E1BAEC5391D69A956BCC");
