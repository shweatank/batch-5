#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>

int mvar_cnt=1;
module_param(mvar_cnt,int,0644);

static int __init simple_fun1_init(void)
{
	printk(KERN_INFO"hello from %s\n",__FUNCTION__);
	for(int i=0;i<mvar_cnt;i++) 
	{
		pr_info("helloo..from loop..index:%d\n",i);
	}
	return 0;
}

static void __exit simple_fun1_exit(void)
{
	pr_info("hello from %s\n",__FUNCTION__);
	for(int i=0;i<mvar_cnt;i++)
        {
                pr_info("from loop..changed val after insmod..index:%d\n",i);
        }

}

module_init(simple_fun1_init);
module_exit(simple_fun1_exit); //macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("prasanna");
MODULE_DESCRIPTION("simple module example");

/*
sudo insmod make_var_module_par.ko mvar_cnt=6
hello from simple_fun1_init
[22990.422220] helloo..from loop..index:0
[22990.422222] helloo..from loop..index:1
[22990.422224] helloo..from loop..index:2
[22990.422225] helloo..from loop..index:3
[22990.422227] helloo..from loop..index:4
[22990.422228] helloo..from loop..index:5
sudo rmmod make_var_module_par.ko
hello from simple_fun1_exit
*/

/*
sudo insmod make_var_module_par.ko
echo 5 |sudo tee /sys/module/make_var_module_par/parameters/mvar_cnt
hello from simple_fun1_init
helloo..from loop..index:0

sudo rmmod make_var_module_par.ko
hello from simple_fun1_exit
from loop..changed val after insmod..index:0
from loop..changed val after insmod..index:1
from loop..changed val after insmod..index:2
from loop..changed val after insmod..index:3
from loop..changed val after insmod..index:4
*/
