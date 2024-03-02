BUILD_DIR = bin

build:
	mkdir -p bin
	gcc -o bin/server.o server.c
