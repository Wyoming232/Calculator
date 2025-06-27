#
# Makefile for the Calculator Project
#

# --- Compiler and Flags ---
# CC: The C compiler to use (gcc is standard)
# CFLAGS: Flags to pass to the compiler
#   -g  : Include debugging symbols (for GDB)
#   -Wall: Warn about all common programming errors
#   -Iinclude: Tell gcc to look for header files in the 'include' directory
CC = gcc
CFLAGS = -g -Wall -Iinclude

# --- Project Files ---
# EXECUTABLE: The name of the final program
# SRCS: A list of all your .c source files
# OBJS: A list of the object files (.o) that will be created from your source files.
#       The ':$(SRCS:.c=.o)' automatically converts the list of .c files to .o files.
EXECUTABLE = calculator
SRCS = src/main.c src/calculator.c src/helpers.c
OBJS = $(SRCS:.c=.o)

# --- Build Rules ---
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# --- Housekeeping Rules ---
clean:
	rm -f $(OBJS) $(EXECUTABLE)
	
re: clean all

.PHONY: all clean re