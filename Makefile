all: src/main.c src/hanoi.c src/hanoi.h
	mkdir -p target
	gcc src/main.c src/hanoi.c -O3 -o target/hanoi

clean:
	rm -f target/hanoi
