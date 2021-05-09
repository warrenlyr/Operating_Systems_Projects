CC=gcc
TARGET=InstaQuack
CFLAGS=-lpthread -pthread

InstaQuack : list.o quacker.o queue.o entry.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean :
	rm -rf *.o InstaQuack

clean_txt :
	rm -rf *.txt
