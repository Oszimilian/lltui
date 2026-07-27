#include "lltui_cursor.h"

#include "lltui_assert.h"
#include "lltui_makros.h"

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

    static char str[32] = {0};
    char* insert = &str[0];

    *(insert++) = 0x1B;
    *(insert++) = '[';

    uint32_t len = str_attache_number(insert, pos.x);
    str_swap(insert, len);

    insert += len;

    *(insert++) = ';';

    len = str_attache_number(insert, pos.y);
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
    static char str[32] = {0};
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