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

bool lltui_pos_move_together(lltui_pos* start_pos, lltui_pos* end_pos) {
    int32_t diff_x = lltui_pos_diff_x(*start_pos, *end_pos);
    int32_t diff_y = lltui_pos_diff_y(*start_pos, *end_pos);

    if (diff_x == 0 && diff_y == 0) return true;

    if (diff_x < 0) {
        lltui_pos_move_right(start_pos);
    } else if(diff_x > 0) {
        lltui_pos_move_left(start_pos);
    } else if(diff_y < 0) {
        lltui_pos_move_down(start_pos);
    } else if(diff_y > 0) {
        lltui_pos_move_up(start_pos);
    }

    return false;
}

lltui_pos lltui_pos_get_corner(lltui_pos start_pos, lltui_pos end_pos, lltui_corner_type type) {
    lltui_pos pos;

    switch (type)
    {
        case lltui_up_left:     pos = start_pos;                                        break;
        case lltui_up_right:    pos = (lltui_pos){.x = end_pos.x, .y = start_pos.y};    break;
        case lltui_down_left:   pos = (lltui_pos){.x = start_pos.x, .y = end_pos.y};    break;
        case lltui_down_right:  pos = end_pos;
        default: break;
    }

    return pos;
}