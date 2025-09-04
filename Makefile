.POSIX:

CC = clang
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -D_POSIX_C_SOURCE=200809L
CFLAGS = -Wall -Wextra -std=c99 -ggdb -O3

ARENA_SRC = arena.c
STRING_SRC = string.c
MAIN_SRC = main.c
ARENA_OBJ = arena.o
STRING_OBJ = string.o
MAIN_OBJ = main.o
HEADERS = arena.h string.h
TARGET = main

CPPFLAGS += $(shell if echo "$(CC)" | grep -q clang && [ "`uname -s`" = "Linux" ]; then echo "-fsanitize=address"; fi)

all: $(TARGET)

$(TARGET): $(MAIN_OBJ) $(ARENA_OBJ) $(STRING_OBJ)
	$(CC) $(CPPFLAGS) $(PLATFORM_FLAGS) $(MAIN_OBJ) $(ARENA_OBJ) $(STRING_OBJ) -o $(TARGET) $(LDFLAGS)
	@echo "==> Build complete: $(TARGET)"

$(MAIN_OBJ): $(MAIN_SRC) $(HEADERS)
	@echo "==> Compiling: $(MAIN_SRC)"
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PLATFORM_FLAGS) -c $(MAIN_SRC)

$(ARENA_OBJ): $(ARENA_SRC) arena.h
	@echo "==> Compiling: $(ARENA_SRC)"
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PLATFORM_FLAGS) -c $(ARENA_SRC)

$(STRING_OBJ): $(STRING_SRC) $(HEADERS)
	@echo "==> Compiling: $(STRING_SRC)"
	$(CC) $(CPPFLAGS) $(CFLAGS) $(PLATFORM_FLAGS) -c $(STRING_SRC)

arena: $(ARENA_OBJ)
	@echo "==> Arena module compiled successfully"

string: $(STRING_OBJ)
	@echo "==> String module compiled successfully"

run: $(TARGET)
	./$(TARGET)

clean:
	@echo "Cleaning up object files and executables..."
	rm -f *.o $(TARGET)
	@echo "Clean complete"

help:
	@echo "Available targets:"
	@echo "  all     - Build the main executable (default)"
	@echo "  run     - Build and run the main executable"
	@echo "  arena   - Compile only the arena module"
	@echo "  string  - Compile only the string module"
	@echo "  clean   - Remove all object files and executables"
	@echo "  help    - Show this help message"

.PHONY: all run arena string clean help
