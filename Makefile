# GRR20195689 Eduardo Vudala Senoski

# Compiler options
CC = gcc
CFLAGS = -Wall
LDLIBS = -lm

# Compiler inputs
HEADERS = image.h header.h pixel.h utils.h
OBJECTS = main.o image.o header.o pixel.o utils.o
OUTPUT = mosaico

debug: $(OBJECTS)
	$(CC) -g $(CFLAGS) $(OBJECTS) $(HEADERS) -o $(OUTPUT) $(LDLIBS)

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(HEADERS) -o $(OUTPUT) $(LDLIBS)

main.o: main.c 
	$(CC) -g $(CFLAGS) -c main.c

image.o: image.c image.h
	$(CC) -g $(CFLAGS) -c image.c image.h

header.o: header.c header.h
	$(CC) -g $(CFLAGS) -c header.c header.h

pixel.o: pixel.c pixel.h
	$(CC) -g $(CFLAGS) -c pixel.c pixel.h

utils.o: utils.c utils.h
	$(CC) -g $(CFLAGS) -c utils.c utils.h

clean:
	rm -rf $(OBJECTS)

purge: clean
	rm -rf $(OUTPUT)