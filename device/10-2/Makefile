obj-m += cross_compiler.o

KDIR := /home/user/linux   # This is your path to RPi kernel source

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
