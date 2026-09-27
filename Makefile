# Makefile

PRJ_NAME = lltui
CC = gcc
FLAGS = -Wall -Wextra -g

INC = -Itest

BUILD_DIR = build

SRC =

include src/lltui.mk
include test/test.mk

all: | $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(PRJ_NAME) $(SRC) $(INC) $(FLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

compiledb:
	bear -- $(MAKE) -k all $(addprefix test_,$(TESTS))

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean compiledb