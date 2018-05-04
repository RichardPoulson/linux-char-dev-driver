obj-m:=helloModule.o
UNAME:=$(shell uname -r)
WORKING:=$(shell pwd)

all:
	make -C /lib/modules/${UNAME}/build M=${WORKING} modules
	sudo insmod helloModule.ko

clean:
	# remove the module from the kernel
	sudo rmmod helloModule
	# first, clean up the folder where the module was constructed
	make -C /lib/modules/${UNAME}/build M=${WORKING} clean