#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/sysrq.h>
#include <linux/utsname.h>

static int crash_type = 0;

/*
    crash_type values:
    0 -> Kernel Oops
    1 -> Kernel Panic
    2 -> Stack Overflow
    3 -> SysRq Crash Dump Trigger
*/

module_param(crash_type, int, 0644);
MODULE_PARM_DESC(crash_type,
        "0=Oops, 1=Panic, 2=StackOverflow, 3=CrashDump");

static void stack_overflow_demo(void)
{
    stack_overflow_demo();
}

static void print_analysis_info(void)
{
    pr_info("=====================================\n");
    pr_info(" Crashdump Analysis Triggered\n");
    pr_info(" Kernel Version : %s\n", init_uts_ns.name.release);
    pr_info(" Crash Type     : %d\n", crash_type);
    pr_info("=====================================\n");
}

static int __init crashdump_analysis_init(void)
{
    int *ptr = NULL;

    pr_info("Crashdump Analysis Module Loaded\n");

    print_analysis_info();

    switch (crash_type) {

    case 0:
        pr_info("Triggering Kernel Oops (NULL Pointer)...\n");
        *ptr = 1234;
        break;

    case 1:
        pr_info("Triggering Kernel Panic...\n");
        panic("Manual Kernel Panic Triggered");
        break;

    case 2:
        pr_info("Triggering Stack Overflow...\n");
        stack_overflow_demo();
        break;

    case 3:
        pr_info("Triggering SysRq Crash Dump...\n");
        handle_sysrq('c');
        break;

    default:
        pr_info("Invalid crash_type selected\n");
        break;
    }

    return 0;
}

static void __exit crashdump_analysis_exit(void)
{
    pr_info("Crashdump Analysis Module Removed\n");
}

module_init(crashdump_analysis_init);
module_exit(crashdump_analysis_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Likitha");
MODULE_DESCRIPTION("Unified Crashdump Analysis Project");
