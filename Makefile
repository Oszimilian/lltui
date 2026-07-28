PRJ_NAME = lltui
CC = gcc
FLAGS = -Wall -Wextra -g
SRC = main.c lltui_ctx.c lltui_widget.c lltui_arena.c lltui_cursor.c lltui_pos.c

all:
	$(CC) -o $(PRJ_NAME) $(SRC) $(FLAGS)

compiledb:
	bear -- $(CC) -o $(PRJ_NAME) $(SRC) $(FLAGS)

clean:
	rm -f $(PRJ_NAME)

.PHONY: all clean compiledb