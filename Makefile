CC = gcc
CFLAGS = -g -Wall -Wshadow -Wextra -pedantic -std=c99
PROGRAM = mathwait
SRCS = mathwait.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS) mathwait.h
.PHONY: all clean
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAM) $(OBJS)

help:
	@echo "Usage: make [all|clean|help]"
	@echo "    all:    Build the $(PROGRAM) program"
	@echo "    clean:  Remove build artifacts"
	@echo "    help:   Print this help message"
