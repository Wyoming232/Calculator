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

# The 'all' target is the default one that runs when you just type 'make'.
# It depends on the final executable.
all: $(EXECUTABLE)

# Rule to create the final executable.
# It depends on all the object files. The command links them together.
# '$(CC) -o $@ $^' means "run gcc, outputting a file named like the target (@), using all the prerequisites (^) as input".
$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $^

# Pattern rule to create an object file (.o) from a source file (.c).
# This single rule handles all your .c files.
# It says "to make a .o file, you need the corresponding .c file".
# The command compiles the source file without linking.
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# --- Housekeeping Rules ---

# The 'clean' rule removes all compiled files. It's useful for starting a fresh build.
# The '.PHONY' declaration tells make that 'clean' is a special target that doesn't produce a file named 'clean'.
clean:
	rm -f $(OBJS) $(EXECUTABLE)

# 're' rule to rebuild the project from scratch. It cleans first, then builds.
re: clean all

.PHONY: all clean re