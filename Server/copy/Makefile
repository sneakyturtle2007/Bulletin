# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c23 -Iinclude

# Find all .c files recursively under src/
SRC = $(shell find src -name '*.c')

# Object files: replace .c with .o
OBJ = $(SRC:.c=.o)

# Output executable name
TARGET = my_program

# Default rule: build the executable
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean

