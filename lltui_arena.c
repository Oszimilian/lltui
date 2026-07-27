#include "lltui_widget.h"
#include "lltui_ctx.h"
#include "lltui_assert.h"
#include "string.h"


void lltui_region_create(lltui_region* region, uint32_t size) {
    LLTUI_ASSERT(region == NULL, "lltui_region is NULL");

    region->data = LLTUI_MALLOC(size * sizeof(uint8_t));
    LLTUI_ASSERT(region->data == NULL, "LLTUI_MALLOC in lltui_region_create failed");

    region->occupancy = 0;
    region->capacity = size;
}

void lltui_arena_create(lltui_arena* arena, uint32_t size) {
    LLTUI_ASSERT(arena == NULL, "arena is NULL");

    lltui_region_create(&arena->table, size * 1/4);
    lltui_region_create(&arena->data, size * 3/4);

    arena->current_arena_descriptor = 0;
}

void lltui_region_malloc(lltui_region* region, uint32_t size) {
    LLTUI_ASSERT(region == NULL, "region is NULL");
    LLTUI_ASSERT(region->occupancy + size > region->capacity, "region overflow");

    region->occupancy += size;
}

int32_t lltui_arena_table_insert(lltui_arena* arena) {
    LLTUI_ASSERT(arena == NULL, "lltui_arena is NULL");

    lltui_region_malloc(&arena->table, sizeof(uint32_t));

    return arena->current_arena_descriptor++;
}

void lltui_arena_talbe_set_index(lltui_arena* arena, int32_t descriptor, uint32_t value) {
    LLTUI_ASSERT(arena == NULL, "lltui_arena is NULL");
    LLTUI_ASSERT(arena->table.occupancy < ((uint32_t)descriptor) * 4, "table_set_index invalid descriptor");

    uint32_t* index = (uint32_t*)(arena->table.data + 4 * descriptor);

    *index = value;
}

uint32_t lltui_arena_table_get(lltui_arena* arena, int32_t descriptor) {
    LLTUI_ASSERT(arena == NULL, "lltui_arena is NULL");
    LLTUI_ASSERT(arena->table.occupancy < ((uint32_t)descriptor) * 4, "table_set_index invalid descriptor");

    uint32_t* index = (uint32_t*)(arena->table.data + 4 * descriptor);
    return *index;
}

int32_t lltui_arena_malloc(lltui_arena* arena, uint32_t size) {
    int32_t descriptor = lltui_arena_table_insert(arena);
    lltui_arena_talbe_set_index(arena, descriptor, arena->data.occupancy);
    lltui_region_malloc(&arena->data, size);
    return descriptor;
}


void* lltui_arena_get_ref(lltui_arena* arena, int32_t descriptor) {
    LLTUI_ASSERT(arena == NULL, "lltui_arena is NULL");

    uint32_t offset = lltui_arena_table_get(arena, descriptor);

    return (void*)(arena->data.data + offset);
}

void lltui_region_rst(lltui_region* region) {
    LLTUI_ASSERT(region == NULL, "lltui_region is NULL");
    region->capacity = 0;
}


void lltui_arena_rst(lltui_arena* arena) {
    LLTUI_ASSERT(arena == NULL, "lltui_arena is NULL");
    lltui_region_rst(&arena->table);
    lltui_region_rst(&arena->data);
    arena->current_arena_descriptor= 0;
}




