PRJ_NAME = lltui
CC = gcc
FLAGS = -Wall -Wextra -g
INC =
SRC = 

include src/lltui.mk

ifneq ($(filter test_box compiledb,$(MAKECMDGOALS)),)
include test/test.mk
endif

test_box:
	$(CC) -o $(PRJ_NAME)_test $(SRC) $(INC) $(FLAGS)
	clear
	./$(PRJ_NAME)_test

all:
	$(CC) -o $(PRJ_NAME) $(SRC) $(INC) $(FLAGS)

compiledb:
	bear -- $(CC) -o $(PRJ_NAME) $(SRC) $(INC) $(FLAGS)

clean:
	rm -f $(PRJ_NAME)

.PHONY: all clean compiledb