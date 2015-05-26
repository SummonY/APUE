GXX		:= gcc
CFLAGS		:= -O2 -Wall -lpthread -fomit-frame-pointer
SO		:= libapue.so
INCLUDE_H	:= -I./ ./$(SO)

SOURCES = $(wildcard *.c, chapter[0-9]*/*.c)
OBJECTS = $(patsubst %.c, %, $(SOURCES))


all : $(OBJECTS)

% : %.c
	$(GXX) $^ -o $@ $(CFLAGS) $(INCLUDE_H)

.PHONY: lib, clean
lib : apue_err.c apue.c
	$(GXX) -fPIC -shared -o $(SO) $^	
clean:
	rm $(OBJECTS)
