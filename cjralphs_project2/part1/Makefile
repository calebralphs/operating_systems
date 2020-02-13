obj-m := part1.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: test_part1
	$(MAKE) -C $(KDIR) M=$(PWD) modules

test_part1: test_part1.c
	gcc -g test_part1.c -o test_part1

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f test_part1

