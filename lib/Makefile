CC = gcc
DEBUG = -O
CFLAGS = $(DEBUG) -std=c17

OBJECTS = MenuSupport.o help.o pipe.o wprintf.o textFromXmString.o dbg.o
TARGETS= libXs.a

all: $(TARGETS)

libXs.a: $(OBJECTS)
	ar ruv libXs.a $(OBJECTS)

clobber:
	rm -f *.o *~* *.a 
	rm -f $(TARGETS)

