.PHONY: all clean

all: catch_sys_calls


scanner: catch_sys_calls.c
	gcc -g catch_sys_calls.c -o catch_sys_calls

clean:
	rm -f catch_sys_calls