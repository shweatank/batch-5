obj-m += iaq.o

iaq-objs := iaq_main.o \
            iaq_timer.o \
            iaq_worker.o \
            iaq_thread.o \
            iaq_buffer.o \
            iaq_dts.o \
            iaq_socket.o

KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

