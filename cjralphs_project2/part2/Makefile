obj-m := part2.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: procAncestry
	$(MAKE) -C $(KDIR) M=$(PWD) modules

procAncestry: procAncestry.c part2.h
	gcc -g procAncestry.c -o procAncestry

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f procAncestry

