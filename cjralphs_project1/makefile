.PHONY: all clean

all: boring custom multi

boring: boring.c boring.h
	gcc -g boring.c -o boring

custom: custom.c boring.h custom.h
	gcc -g custom.c -o custom

multi: multi.c multi.h boring.h custom.h
	gcc -g multi.c -o multi

clean:
	rm -f boring custom multi