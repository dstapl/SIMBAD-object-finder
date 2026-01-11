.PHONY: clean, run, all, rebuild

CC := gcc

NAME := astro_transit

SRC_DIR := src
OBJ_DIR := build


SRCS := $(wildcard $(SRC_DIR)/*.c)

# Replace src/*.c with build/*.o
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HFILES := $(wildcard $(SRC_DIR)/*.h)

# Only include libxml*/include?
# See https://github.com/servo/libexpat/blob/main/expat/win32/README.txt
EXPAT_DIR = lib/expat-2.7.3
SQLITE_DIR = lib/sqlite-3.51.1

INCLDIR ?= include $(EXPAT_DIR)/src/lib $(SQLITE_DIR)
LIBDIR ?= $(EXPAT_DIR)/lib $(SQLITE_DIR)
LINKFILES ?= expat sqlite3
INCLFLAGS ?= $(INCLDIR:%=-I%) $(LIBDIR:%=-L%)

CSTD ?= c99
CFLAGS ?= -g -std=$(CSTD) -Wall -Wextra -Werror $(INCLFLAGS)


# Make rules
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFILES:%=-l%)

$(NAME).exe: $(NAME)

# Compile .c from src to .o in build
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HFILES) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LINKFILES:%=-l%)

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
