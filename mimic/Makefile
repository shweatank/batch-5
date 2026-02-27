//obj-m += mimic_sensor.o
//obj-m += test_mimic.o
//obj-m += mimic_safe.o
//obj-m += mimic_unload.o
//obj-m += final.o
//obj-m += clean.o
//obj-m += f.o
//obj-m += f1.o
//obj-m += uart_pro.o
obj-m += final_mimic_sensor.o
//obj-m += test.o
obj-m += fin.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean                                
