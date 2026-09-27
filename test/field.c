#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
#include "lltui.h"
#include "lltui_assert.h"
#include "lltui_cursor.h"


int main() {

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    lltui_ctx ctx = {0};
    ctx.cb.tx_cb = print_lltui_buffer;
    ctx.cb.rx_cb = read_lltui_buffer;

    lltui_ctx_init(&ctx, 2048);

    lltui_pos start_pos = LLTUI_POS(1, 1);
    lltui_pos end_pos = LLTUI_POS(20, 1);

    int32_t field1 = lltui_widget_create(&ctx, start_pos, end_pos, lltui_textfield);
        
    lltui_widget_set_text(&ctx, field1, "Hallo World!");

    lltui_print(&ctx);

    sleep(1);

    lltui_widget_color_background(&ctx, field1, LLTUI_BLUE);

    lltui_print(&ctx);

    sleep(1);

    lltui_widget_color_foreground(&ctx, field1, LLTUI_GREEN);

    lltui_print(&ctx);

    return 0;
}