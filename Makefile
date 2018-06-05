
ifneq ($(KERNELRELEASE),)
	obj-m := chario.o
else 
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

defalt : 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif


