# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g	

# Automatically find all C source files
SOURCES = $(wildcard *.c)

# Generate executable names from source files (without the .c extension)
TARGETS = $(SOURCES:.c=)

# Default target
all: $(TARGETS)

# Rule to compile each source file into an executable
%: %.c
	$(CC) $(CFLAGS) -o ../../build/$@ $<

# Clean up generated files
clean:
	rm -f $(TARGETS)

# Phony targets
.PHONY: all clean
