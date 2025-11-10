CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
INCLUDES = -Iinclude
LIBS =

SRCS = src/tpool.c src/example.c
OBJS = $(SRCS:.c=.o)
TARGET = example

all: $(TARGET)

$(TARGET): src/tpool.o src/example.o
    $(CC) $(CFLAGS) $(INCLUDES) -o $(TARGET) src/tpool.o src/example.o $(LIBS)

src/%.o: src/%.c
    $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
    rm -f src/*.o $(TARGET)

.PHONY: all clean
