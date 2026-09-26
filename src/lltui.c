#include "lltui.h" 

void lltui_print(lltui_ctx* ctx) {
    for (uint32_t i = 0; i < ctx->widget_arena.current_arena_descriptor; i++) {
        lltui_widget_print(ctx, i);
    }
}