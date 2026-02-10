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
	{ 0xecaf41c4, "dma_free_attrs" },
	{ 0x2b21d1e1, "dma_release_channel" },
	{ 0x92997ed8, "_printk" },
	{ 0xa6257a2f, "complete" },
	{ 0x61fd46a9, "platform_driver_unregister" },
	{ 0x36a78de3, "devm_kmalloc" },
	{ 0x608741b5, "__init_swait_queue_head" },
	{ 0x4ac34dbd, "dma_request_chan" },
	{ 0xa3b07fba, "dma_alloc_attrs" },
	{ 0xdcb764ad, "memset" },
	{ 0x4a3ad70e, "wait_for_completion_timeout" },
	{ 0x449ad0a7, "memcmp" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Ctechdhaba,dma-test");
MODULE_ALIAS("of:N*T*Ctechdhaba,dma-testC*");

MODULE_INFO(srcversion, "16167AC7727BE301E5C0AE1");
