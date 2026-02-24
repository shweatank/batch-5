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
	{ 0x80192c9b, "of_property_read_variable_u32_array" },
	{ 0xc5b6f236, "queue_work_on" },
	{ 0x92540fbf, "finish_wait" },
	{ 0xea82d349, "hrtimer_init" },
	{ 0x37a0cba, "kfree" },
	{ 0x1114bbcd, "of_find_node_by_name" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xe2964344, "__wake_up" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb2cf531a, "wake_up_process" },
	{ 0x92997ed8, "_printk" },
	{ 0x1000e51, "schedule" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xea3c74e, "tasklet_kill" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xee616474, "init_net" },
	{ 0x2364c85a, "tasklet_init" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x9d2ab8ac, "__tasklet_schedule" },
	{ 0xd45b3bc6, "kthread_stop" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0x4ac51a2, "kernel_connect" },
	{ 0xdcb764ad, "memset" },
	{ 0xc0b7c197, "hrtimer_start_range_ns" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xcb661d89, "kthread_create_on_node" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x145b5268, "sock_create_kern" },
	{ 0x5443de3e, "__kmalloc_cache_noprof" },
	{ 0xb43f9365, "ktime_get" },
	{ 0x3c12dfe, "cancel_work_sync" },
	{ 0xdf39592e, "sock_release" },
	{ 0x102fe6de, "hrtimer_cancel" },
	{ 0xa65c6def, "alt_cb_patch_nops" },
	{ 0x135bb7ec, "hrtimer_forward" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0xe4929c22, "kernel_sendmsg" },
	{ 0xd90f6665, "kmalloc_caches" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x474e54d2, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "8F3B236010F1AE27127C005");
