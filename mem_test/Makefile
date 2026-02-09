obj-m += mem_test.o

KDIR := /home/mirafra/linux

all:
    make -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules

clean:
    make -C $(KDIR) M=$(PWD) clean

