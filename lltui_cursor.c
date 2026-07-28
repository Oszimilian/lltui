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

void lltui_cursor_clear_line(lltui_ctx* ctx, lltui_pos start_pos, lltui_pos end_pos){
    uint32_t diff_x = lltui_pos_diff_x(start_pos, end_pos);
    uint32_t diff_y = lltui_pos_diff_y(start_pos, end_pos);

    uint32_t diff = (diff_x == 0) ? diff_y : diff_x;

    uint8_t direction = 0;
    if (diff_x > 0) direction = lltui_right;
    else if (diff_y > 0) direction = lltui_down;
    else if (diff_x < 0) direction = lltui_left;
    else if (diff_y < 0) direction = lltui_up;

    for (uint32_t i = 0; i < diff; i++) {
        lltui_cursor_move(ctx, start_pos);

        switch (direction) {
            case lltui_up:      lltui_pos_move_up(&start_pos);      break;
            case lltui_down:    lltui_pos_move_down(&start_pos);    break;
            case lltui_right:   lltui_pos_move_right(&start_pos);   break;
            case lltui_left:    lltui_pos_move_left(&start_pos);    break;
        
            default: break;
        }

        char* insert = &str[0];
        *insert = 0x7F;

        ctx->cb.tx_cb(str, (uint32_t)(insert - str));
    }


}