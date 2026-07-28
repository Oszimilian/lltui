#include "lltui_pos.h"

uint32_t lltui_pos_abs_diff_x(lltui_pos start_pos, lltui_pos end_pos) {
    int32_t diff = start_pos.x - end_pos.x;
    return lltui_abs_i32(diff);
}

uint32_t lltui_pos_abs_diff_y(lltui_pos start_pos, lltui_pos end_pos) {
    int32_t diff = start_pos.y - end_pos.y;
    return lltui_abs_i32(diff);
}

int32_t lltui_pos_diff_x(lltui_pos start_pos, lltui_pos end_pos) {
    return start_pos.x - end_pos.x;
}

int32_t lltui_pos_diff_y(lltui_pos start_pos, lltui_pos end_pos) {
    return start_pos.y - end_pos.y;
}

void lltui_pos_move_up(lltui_pos* pos) {
    pos->y--;
}

void lltui_pos_move_down(lltui_pos* pos) {
    pos->y++;
}

void lltui_pos_move_right(lltui_pos* pos) {
    pos->x++;
}

void lltui_pos_move_left(lltui_pos* pos) {
    pos->x--;
}

int32_t lltui_abs_i32(int32_t value) {
    return (value > 0) ? value : -value; 
}

lltui_pos lltui_pos_get_lowest(lltui_pos pos_a, lltui_pos pos_b) {
    return (pos_a.y > pos_b.y) ? pos_a : pos_b;
}