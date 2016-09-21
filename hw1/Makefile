CC=gcc
CFLAGS=-I -Wall

DEPS = hellofunc.h

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

hellomake: hellomake.o hellofunc.o
    $(CC) -o hello hellomake.o hellofunc.o -I.

clean:
    rm -f *.o *~ core* hello