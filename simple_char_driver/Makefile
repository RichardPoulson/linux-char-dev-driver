obj-m:=simple_char_driver.o
UNAME:=$(shell uname -r)
WORKING:=$(shell pwd)

all:
	make -C /lib/modules/${UNAME}/build M=${WORKING} modules
	sudo insmod simple_char_driver.ko
	make test_driver
	sudo make device

clean:
	rm -f ${WORKING}/test_driver
	sudo rm -f /dev/simple_character_device
	# first, clean up the folder where the module was constructed
	make -C /lib/modules/${UNAME}/build M=${WORKING} clean
	# remove the module from the kernel
	sudo rmmod simple_char_driver

device:
	sudo mknod -m 777 /dev/simple_character_device c 240 0

test_driver: test_driver.c
	gcc test_driver.c -o test_driver
