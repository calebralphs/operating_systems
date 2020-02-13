obj-m := part1.o part2.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: procAncestry test_part1
	$(MAKE) -C $(KDIR) M=$(PWD) modules

procAncestry: procAncestry.c part2.h
	gcc -g procAncestry.c -o procAncestry

test_part1: test_part1.c
	gcc -g test_part1.c -o test_part1

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

