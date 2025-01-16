# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iexternal/raylib/include

# Raylib library paths
LDFLAGS = -Lexternal/raylib/lib -lraylib -lm -lpthread -ldl -lX11

# Project-specific sources
PROJECT_SRCS = main.c game.c ui.c
PROJECT_EXE = conway

# Build all targets
all: $(PROJECT_EXE)


# Rule for the main project executable
$(PROJECT_EXE): $(PROJECT_SRCS)
	$(CC) $(CFLAGS) -o $@ $(PROJECT_SRCS) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(ACT1) $(ACT2) $(ACT3) $(ACT4) $(PROJECT_EXE)

# Phony targets
.PHONY: all clean
