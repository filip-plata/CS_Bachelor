GXX = g++
FLAGS= -O2 -Wall -g

client:
	$(GXX) $(FLAGS) ../client.cc -o client

server:
	$(GXX) $(FLAGS) ../server.cc -o server

all: server client

clear:
	rm -f client server *.o

.PHONY: server client
