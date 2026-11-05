# Makefile para Fixly - Programación I (UTN INSPT)
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = fixly
SRC = main.c
HEADERS = utiles.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
