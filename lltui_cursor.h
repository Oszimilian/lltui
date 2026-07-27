#ifndef _LLTUI_CURSOR_H_
#define _LLTUI_CURSOR_H_

#include <inttypes.h>
#include "lltui_pos.h"
#include "lltui_ctx.h"
#include "lltui_widget.h"



uint32_t str_attache_number(char* str, uint32_t pos);
void str_swap(char* str, uint32_t len);

void lltui_cursor_move(lltui_ctx* ctx, lltui_pos pos);
void lltui_cursor_clear_window(lltui_ctx* ctx);
void lltui_cursor_color(lltui_ctx* ctx, lltui_color color);


#endif