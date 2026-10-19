CC = gcc
CFLAGS = -Wall -Wextra -g
LDLIBS = -lm
SOURCES = main.c interface.c storage.c vector.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = minimat

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS)

%.o: %.c vector.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean