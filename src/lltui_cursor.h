#ifndef _LLTUI_CURSOR_H_
#define _LLTUI_CURSOR_H_

#include <inttypes.h>
#include "lltui_pos.h"
#include "lltui_ctx.h"
#include "lltui_widget.h"



void lltui_cursor_move(lltui_ctx* ctx, lltui_pos pos);
void lltui_cursor_clear_window(lltui_ctx* ctx);
void lltui_cursor_color(lltui_ctx* ctx, lltui_color color);
void lltui_cursor_clear_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos);
void lltui_cursor_draw_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos);
void lltui_cursor_draw_corner(lltui_ctx* ctx, lltui_corner_type type);

#endif