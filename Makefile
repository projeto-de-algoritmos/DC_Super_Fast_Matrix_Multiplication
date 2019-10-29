CC := g++
CFLAGS := main.cpp -std=c++11 -O3 -lpthread -I/usr/include/python2.7 -lpython2.7 -o main

all:
	$(CC) $(CFLAGS)