CC=gcc
CFLAGS=-I -Wall

DEPS = hellofunc.h

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

make: timing.o
    $(CC) -o timing

clean:
    rm -f *.o *~ core* hello