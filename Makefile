CC=gcc

all: keylog

keylog: keylog.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<

clean:
	rm -f core *.o *.txt keylog
