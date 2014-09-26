all: main.c hanoi.c hanoi.h
	gcc main.c hanoi.c -O3 -o hanoi

clean:
	rm -f hanoi
