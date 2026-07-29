#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "lltui.h"
#include "lltui_assert.h"
#include "lltui_cursor.h"

void print_lltui_buffer(char* buf, uint32_t len) {
    
    for (uint32_t i = 0; i < len; i++) {
        if (buf != NULL) {
            putchar(buf[i]);
        } else {
            putchar(' ');
        }
    }

    fflush(stdout);
}

void read_lltui_buffer(char** buf, uint32_t* len, uint32_t max_len) {
    LLTUI_ASSERT(buf == NULL, "buf is NULL");

    *len = read(STDIN_FILENO, *buf, max_len);
}

int main() {

    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);


    lltui_ctx ctx = {0};
    ctx.cb.tx_cb = print_lltui_buffer;
    ctx.cb.rx_cb = read_lltui_buffer;

    lltui_ctx_init(&ctx, 2048);
    
    lltui_pos start_pos = {.x = 2, .y = 2};
    lltui_pos end_pos   = {.x = 20, .y = 2};

    int32_t d = lltui_widget_create(&ctx, start_pos, end_pos, lltui_textfield);
    lltui_widget_set_text(&ctx, d, "Hallo");

    lltui_pos_move_down(&start_pos);
    lltui_pos_move_down(&end_pos);
    int32_t d2 = lltui_widget_create(&ctx, start_pos, end_pos, lltui_textfield);




    lltui_pos box_a = {.x = 1, .y = 1};
    lltui_pos box_b = {.x = 50, .y = 10};
    int32_t d5 = lltui_widget_create(&ctx, box_a, box_b, lltui_box);

    lltui_widget_set_text(&ctx, d2, "Maximilian");


    lltui_cursor_clear_window(&ctx);

     
    lltui_widget_print(&ctx, d);
    lltui_widget_print(&ctx, d2);
    lltui_widget_print(&ctx, d5);


    


    lltui_widget_set_text(&ctx, d, "Oszi");

    lltui_widget_print(&ctx, d);
    lltui_widget_print(&ctx, d2);



    lltui_ctx_destroy(&ctx);
    

    return 0;
}