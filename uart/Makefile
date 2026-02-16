obj-m += uart_2_com.o
obj-m += uart_2_rpi_com.o
obj-m += uart_dts_driver.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
