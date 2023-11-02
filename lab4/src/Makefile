#CC = gcc
#CFLAGS = -Wall

#all: parallel_min_max process_memory

#parallel_min_max: upd_parallel_min_max.c
#$(CC) $(CFLAGS) -o $@ $<

#process_memory: process_memory.c
#$(CC) $(CFLAGS) -o $@ $<

#clean:
#rm -f parallel_min_max process_memory


#для parallel_sum
CC = gcc
CFLAGS = -Wall -pthread

all: parallel_sum

parallel_sum: parallel_sum.c utils.o
	$(CC) $(CFLAGS) -o parallel_sum parallel_sum.c utils.o

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f parallel_sum utils.o