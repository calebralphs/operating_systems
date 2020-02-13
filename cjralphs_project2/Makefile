obj-m := part1.o part2.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: procAncestors test_part1
	$(MAKE) -C $(KDIR) M=$(PWD) modules

procAncestors: procAncestors.c part2.h
	gcc -g procAncestors.c -o procAncestors

test_part1: test_part1.c
	gcc -g test_part1.c -o test_part1

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

