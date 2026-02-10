obj-m += cross_driver.o
KDIR := /home/mirafra/rpi/linux


ARCH := arm64
CROSS_COMPILE := aarch64-linux-gnu-

PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) \
	ARCH=$(ARCH) \
	CROSS_COMPILE=$(CROSS_COMPILE) \
	M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
