#include "lltui_ctx.h"
#include "lltui_assert.h"
#include "lltui_print.h"
#include "lltui_ctx.h"
#include "lltui_cursor.h"

void lltui_ctx_init(lltui_ctx* ctx, uint32_t size) {
    LLTUI_ASSERT(ctx == NULL, "lltui ctx is NULL");
    LLTUI_ASSERT(ctx->cb.tx_cb == NULL, "lltui txcb is missing");
    LLTUI_ASSERT(ctx->cb.rx_cb == NULL, "lltui rxcb is missing");

    LLTUI_PRINTF("LLTUI_CTX_INIT \n");

    lltui_arena_create(&ctx->widget_arena, size);
    
    ctx->lowest_pos = (lltui_pos){.x = 1, .y = 1};

    ctx->current_descriptor = 0;
}

void lltui_ctx_destroy(lltui_ctx* ctx) {
    lltui_pos_move_down(&ctx->lowest_pos);
    lltui_pos_move_down(&ctx->lowest_pos);
    lltui_cursor_move(ctx, ctx->lowest_pos);
}