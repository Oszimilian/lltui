#ifndef _LLTUI_CTX_H_
#define _LLTUI_CTX_H_


#include "lltui_arena.h"
#include "lltui_pos.h"


typedef struct {
    void (*tx_cb)(char* buf, uint32_t len);
    void (*rx_cb)(char** buf, uint32_t* len, uint32_t max_len);
}lltui_cb;

typedef struct {
    lltui_arena widget_arena;
}lltui_area;

typedef struct lltui_ctx {
    lltui_cb cb;
    lltui_arena widget_arena;
    int32_t current_descriptor;
    lltui_pos lowest_pos;
}lltui_ctx;

void lltui_ctx_init(lltui_ctx* ctx, uint32_t size);
void lltui_ctx_destroy(lltui_ctx* ctx);
#endif