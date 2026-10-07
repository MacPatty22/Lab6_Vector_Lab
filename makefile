CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic
OBJECTS = main.o vector.o storage.o interface.o

minimat: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o minimat

%.o: %.c vector.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS) minimat