# Define variables
CC = gcc
CFLAGS = -Wall -g
OBJS = main.o helpers.o
TARGET = my_app

# Automatically find all .c files in the current directory
SRCS = $(wildcard *.c)
# Create a list of object files by replacing .c with .o
OBJS = $(SRCS:.c=.o)

# Use variables in rules
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.c helpers.h
	$(CC) $(CFLAGS) -c main.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c
