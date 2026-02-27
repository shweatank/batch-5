//obj-m := simple_module.o

//obj-m += export_symb.o
//obj-m += export_symb_using_fun.o
//obj-m += make_var_module_par.o
//obj-m += char_driver.o
//obj-m += demo_irq_driver.o
//obj-m += keyboard_irq_driver.o
//obj-m += timer_interrupt.o
//obj-m += timer_tasklet.o
//obj-m += work_queue.o
obj-m += kmemleak.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
