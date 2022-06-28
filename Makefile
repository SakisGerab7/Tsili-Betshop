CC = gcc
CFLAGS = -Wall -g -fsanitize=address
# CFLAGS = -Wall -g -fsanitize=address -DDEBUG
SRC = $(wildcard src/*.c)
OBJDIR = $(shell mkdir objs)
OBJS = $(patsubst src/%.c, objs/%.o, $(SRC))
BIN = tsili

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN)

objs/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -r $(OBJS)
