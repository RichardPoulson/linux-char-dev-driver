UNAME:=$(shell uname -r)
WORKING:=$(shell pwd)

all:
	sudo make -C ${WORKING}/simple_char_driver -f ${WORKING}/simple_char_driver/Makefile

hello:
	sudo make -C ${WORKING}/helloModule -f ${WORKING}/helloModule/Makefile

test:
	${WORKING}/simple_char_driver/test_driver

clean:
	sudo make -C ${WORKING}/simple_char_driver -f ${WORKING}/simple_char_driver/Makefile clean -k

clean_hello:
	sudo make -C ${WORKING}/helloModule -f ${WORKING}/helloModule/Makefile clean -k