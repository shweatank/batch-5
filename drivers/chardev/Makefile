obj-m += chardev.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

install:
	sudo cp chardev.ko /lib/modules/$(shell uname -r)/extra/
	sudo depmod -a

clean:
	make -C $(KDIR) M=$(PWD) clean
