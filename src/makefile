#Name: Hubert Iwanow

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Directories
SRC_DIR = .
BIN_DIR = ../bin

# Target executable
TARGET = $(BIN_DIR)/customShell

# Source and object files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/utility.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files into the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
