all: main.c hanoi.c hanoi.h
	gcc main.c hanoi.c -o hanoi

clean:
	rm -f hanoi
