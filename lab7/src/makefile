CC = gcc
CFLAGS = -Wall
TARGETS = tcpclient tcpserver udpclient udpserver

all: $(TARGETS)

tcpclient: tcpclient.c
	$(CC) $(CFLAGS) -o $@ $^

tcpserver: tcpserver.c
	$(CC) $(CFLAGS) -o $@ $^

udpclient: udpclient.c
	$(CC) $(CFLAGS) -o $@ $^

udpserver: udpserver.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
