obj-m += irq_waitqueue_example.o
obj-m += irq_driver_cal_task.o
obj-m += proc_basic_driver.o
obj-m += proc_strrev_driver.o
obj-m += proc_calc_driver.o
obj-m += basic_sys_fs_driver.o
obj-m += irq_invoking_work_queue.o
obj-m += kbd_irq_workqueue_calc_driver.o
obj-m += proc_kbd_irq_workqueue_calc_driver.o


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
