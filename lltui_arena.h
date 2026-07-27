#ifndef _LLTUI_ARENA_H_
#define _LLTUI_ARENA_H_

#include <stdint.h>

#include "lltui_assert.h"
#include "lltui_config.h"

typedef struct lltui_ctx lltui_ctx;
typedef struct lltui_widget lltui_widget;



typedef struct {
    void* data;
    uint32_t occupancy;
    uint32_t capacity;
}lltui_region;

typedef struct {
    lltui_region table;
    lltui_region data;
    int32_t current_arena_descriptor;
}lltui_arena;

void lltui_region_create(lltui_region* region, uint32_t size);
void lltui_arena_create(lltui_arena* arena, uint32_t size);

void lltui_region_malloc(lltui_region* region, uint32_t size);
int32_t lltui_arena_malloc(lltui_arena* arena, uint32_t size) ;

int32_t lltui_arena_table_insert(lltui_arena* arena);
void lltui_arena_talbe_set_index(lltui_arena* arena, int32_t descriptor, uint32_t value);
uint32_t lltui_arena_table_get(lltui_arena* arena, int32_t descriptor);
void* lltui_arena_get_ref(lltui_arena* arena, int32_t descriptor);

void lltui_region_rst(lltui_region* region);
void lltui_arena_rst(lltui_arena* arena);

#endif  