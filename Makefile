.PHONY: clean, run, all, rebuild

CC := gcc

NAME := astro_transit

SRC_DIR := src
OBJ_DIR := build


SRCS := $(wildcard $(SRC_DIR)/*.c)

# Replace src/*.c with build/*.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HFILES := $(wildcard $(SRC_DIR)/*.h)

INCLDIR ?= include
LIBDIR ?= lib
INCLFLAGS ?= $(INCLDIR:%=-I%) $(LIBDIR:%=-L%)

CSTD ?= c99
CFLAGS ?= -g -std=$(CSTD) -Wall -Wextra -Werror $(INCLFLAGS)


# Make rules
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

$(NAME).exe: $(NAME)

# Compile .c from src to .o in build
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HFILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(OBJ_DIR):
	if NOT EXIST $(OBJ_DIR) mkdir $(OBJ_DIR)

run: $(NAME).exe
	$(NAME).exe

clean:
ifeq ($(OS),Windows_NT)
	@cmd /c "del /F $(NAME).exe"
	@cmd /c "if exist $(OBJ_DIR) del /Q /S $(OBJ_DIR)"
	cmd /c "type nul > $(OBJ_DIR)\.gitkeep"
else
	rm -rf $(OBJ_DIR) $(NAME)
	touch $(OBJ_DIR)/.gitkeep
endif

rebuild: clean all
