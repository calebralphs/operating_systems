.PHONY: all clean

all: scanner catch_sys_calls

scanner: scanner.c
	gcc -g scanner.c -o scanner

scanner: catch_sys_calls.c
	gcc -g catch_sys_calls.c -o catch_sys_calls

clean:
	rm -f scanner catch_sys_calls