# Makefile for character device driver and test program

# Kernel module build
obj-m := my_char.o

# Get current kernel version
KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

# Default target - build both driver and test program
all: driver test

# Build the kernel module
driver:
	make -C $(KDIR) M=$(CURDIR) modules

# Build the test program
test: test_char

test_char: test_char.c
	gcc -o test_char test_char.c

# Clean targets
clean:
	make -C $(KDIR) M=$(CURDIR) clean
	rm -f test_char

# Install and test targets
install: driver
	sudo insmod my_char.ko
	$(MAKE) create-nodes

uninstall:
	sudo rmmod my_char

# Create device nodes manually if needed
create-nodes:
	sudo mknod /dev/my_char0 c 42 0
	sudo mknod /dev/my_char1 c 42 1
	sudo mknod /dev/my_char2 c 42 2
	sudo mknod /dev/my_char3 c 42 3
	sudo mknod /dev/my_char4 c 42 4
	sudo chmod 666 /dev/my_char*

# Run the test
run-test: test
	./test_char

# Full test sequence
full-test: driver install run-test

.PHONY: all driver test clean install uninstall create-nodes run-test full-test