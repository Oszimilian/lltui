#ifndef _LLTUI_POS_H_
#define _LLTUI_POS_H_

#include <inttypes.h>
#include <stdbool.h>

typedef enum {
    lltui_up,
    lltui_down,
    lltui_right,
    lltui_left
}lltui_direction;

typedef struct {
    uint32_t x;
    uint32_t y;
}lltui_pos;


uint32_t lltui_pos_abs_diff_x(lltui_pos start_pos, lltui_pos end_pos);
uint32_t lltui_pos_abs_diff_y(lltui_pos start_pos, lltui_pos end_pos);

int32_t lltui_pos_diff_x(lltui_pos start_pos, lltui_pos end_pos);
int32_t lltui_pos_diff_y(lltui_pos start_pos, lltui_pos end_pos);

void lltui_pos_move_up(lltui_pos* pos);
void lltui_pos_move_down(lltui_pos* pos);
void lltui_pos_move_right(lltui_pos* pos);
void lltui_pos_move_left(lltui_pos* pos);

int32_t lltui_abs_i32(int32_t value);

lltui_pos lltui_pos_get_lowest(lltui_pos pos_a, lltui_pos pos_b);

bool lltui_pos_move_together(lltui_pos* start_pos, lltui_pos* end_pos);

#endif