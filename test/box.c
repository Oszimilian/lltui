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


    lltui_pos box_a = LLTUI_POS(1, 1);
    lltui_pos box_b = LLTUI_POS(20, 10);

    lltui_pos box_c = LLTUI_POS(25, 1);
    lltui_pos box_d = LLTUI_POS(45, 10);

    int32_t box = lltui_widget_create(&ctx, box_a, box_b, lltui_box);
    int32_t box2 = lltui_widget_create(&ctx, box_c, box_d, lltui_box);
    lltui_widget_shadow(&ctx, box2);

    lltui_print(&ctx);

    sleep(1);

    lltui_widget_show(&ctx, box2);
    lltui_widget_shadow(&ctx, box);

    lltui_print(&ctx);

    sleep(1);

    lltui_widget_color_background(&ctx, box, LLTUI_BLUE);
    lltui_widget_show(&ctx, box);

    lltui_print(&ctx);

    sleep(1);

    lltui_widget_color_foreground(&ctx, box2, LLTUI_BLUE);

    lltui_print(&ctx);

    lltui_ctx_destroy(&ctx);
    

    return 0;
}