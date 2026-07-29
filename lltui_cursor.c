#include "lltui_cursor.h"

#include "lltui_assert.h"
#include "lltui_makros.h"
#include "lltui_pos.h"

static char str[128] = {0};


uint32_t str_attache_number(char* str, uint32_t pos) {
    uint32_t len = 0;

    do{
        str[len++] = (char)('0' + pos % 10);
        pos /= 10;
    }while(pos > 0);

    return len;
}

void str_swap(char* str, uint32_t len) {
    int i = 0;
    int k = len - 1;

    for (; i < k; i++, k--) {
        char tmp = str[i];
        str[i] = str[k];
        str[k] = tmp;
    }
}

void lltui_cursor_move(lltui_ctx* ctx, lltui_pos pos) {
    LLTUI_ASSERT(ctx == NULL, "ctx is NULL");

    char* insert = &str[0];

    *(insert++) = 0x1B;
    *(insert++) = '[';

    uint32_t len = str_attache_number(insert, pos.y);
    str_swap(insert, len);

    insert += len;

    *(insert++) = ';';

    len = str_attache_number(insert, pos.x);
    str_swap(insert, len);

    insert += len;

    *(insert++) = 'H';
    *(insert) = '\0';

    ctx->cb.tx_cb(str, (uint32_t)(insert - str) + 1);
}

void lltui_cursor_clear_window(lltui_ctx* ctx) {
    char cmd[] = "\e[2J\e[H";
    ctx->cb.tx_cb(cmd, ARRAY_SIZE(cmd) - 1);
}

void lltui_cursor_color(lltui_ctx* ctx, lltui_color color) {
    
    char* insert = &str[0];

    //foreground
    *(insert++) = 0x1B;
    *(insert++) = '[';

    uint32_t len = str_attache_number(insert, color.foreground);
    str_swap(insert, len);

    insert += len;
    *(insert++) = 'm';

    //background
    *(insert++) = 0x1B;
    *(insert++) = '[';

    len = str_attache_number(insert, color.background);
    str_swap(insert, len);

    insert += len;
    *(insert++) = 'm';


    ctx->cb.tx_cb(str, (uint32_t)(insert - str));
}



void lltui_cursor_clear_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos) {
    do{
        lltui_cursor_move(ctx, start_pos);
        ctx->cb.tx_cb(" ", 1);
    }while(lltui_pos_move_together(&start_pos, &end_pos) == false);
}

void lltui_cursor_draw_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos) {
    uint32_t diff_x = lltui_pos_abs_diff_x(start_pos, end_pos);

    do{
        lltui_cursor_move(ctx, start_pos);

        if (diff_x > 0) {
            ctx->cb.tx_cb("\xE2\x94\x80", 3);
        } else {
            ctx->cb.tx_cb("\xE2\x94\x82", 3);
        }
    } while(lltui_pos_move_together(&start_pos, &end_pos) == false);
    
}

void lltui_cursor_draw_corner(lltui_ctx* ctx, lltui_pos pos, lltui_corner_type type) {
    char* insert = &str[0];
    *(insert++) = 0xE2;
    *(insert++) = 0x94;


    switch(type) {
        case lltui_up_left:     *(insert++) = 0x8C; break;
        case lltui_up_right:    *(insert++) = 0x90; break;
        case lltui_down_left:   *(insert++) = 0x94; break;
        case lltui_down_right:  *(insert++) = 0x98; break;
        default: break;
    }

    ctx->cb.tx_cb(str, (uint32_t)(insert - str));
}