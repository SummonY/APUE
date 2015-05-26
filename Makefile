GXX		:= gcc
CFLAGS		:= -O2 -Wall -lpthread -fomit-frame-pointer
INCLUDE_H	:= -I./

SOURCES = $(wildcard *.c, chapter[0-9]*/*.c)
OBJECTS = $(patsubst %.c, %, $(SOURCES))

all : $(OBJECTS)

% : %.c
	$(GXX) $(CFLAGS) $(INCLUDE_H) $^ -o $@

.PHONY: clean
clean:
	rm $(OBJECTS) all
