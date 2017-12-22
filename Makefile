CFLAGS= -g -Wall
CC=gcc

all: TCPClient TCPServer

TCPClient: TCPClient.o
TCPClient.o: TCPClient.c

TCPServer: TCPServer.o
TCPServer.o: TCPServer.c

server:
	./TCPServer

client:
	./TCPClient

clean:
	rm *.o TCPServer TCPClient
