# compiler
CC = gcc

# compiler flags
CFLAGS = -Wall -I./include

# sources
SRCS = $(wildcard ./src/*.c)

# objects
OBJS = $(patsubst ./src/%.c, ./obj/%.o, $(SRCS))

# output
OUT = main

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

./obj/%.o: ./src/%.c | ./obj
	$(CC) $(CFLAGS) -c $< -o $@

./obj:
	mkdir -p ./obj

clean:
	rm -f $(OBJS) $(OUT)

.PHONY: all clean

