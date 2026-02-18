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



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x8da0819, "kmalloc_caches" },
	{ 0xd0c3484c, "kmalloc_trace" },
	{ 0x90aea9a7, "input_register_handle" },
	{ 0xeb496698, "input_open_device" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xf9a482f9, "msleep" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xb78fd6ce, "input_unregister_handler" },
	{ 0x9166fc03, "__flush_workqueue" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xedc03953, "iounmap" },
	{ 0xde80cd09, "ioremap" },
	{ 0xa78af5f3, "ioread32" },
	{ 0x49cd25ed, "alloc_workqueue" },
	{ 0x8bc68549, "input_register_handler" },
	{ 0x864e77e5, "__register_chrdev" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xa1a6b46e, "input_close_device" },
	{ 0x87395c9a, "input_unregister_handle" },
	{ 0x37a0cba, "kfree" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x122c3a7e, "_printk" },
	{ 0x4a453f53, "iowrite32" },
	{ 0xe2fd41e5, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("input:b*v*p*e*-e*1,*2,*k*r*a*m*l*s*f*w*");

MODULE_INFO(srcversion, "3127D9C97FAAD01FB795215");
