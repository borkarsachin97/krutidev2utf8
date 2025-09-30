# Define compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -DDEBUGMODE=0 $(shell pkg-config --cflags glib-2.0)

# Define target executable
TARGET = kruti2utf

# Define source files
SRC = kruti2utf.c 

# Define object files (automatically replaces .c with .o)
OBJ = $(SRC:.c=.o)

# Define linker flags
LIBS = $(shell pkg-config --libs glib-2.0)

# Build the target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compile each .c file into an object file
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)
