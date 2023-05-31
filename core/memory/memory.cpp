
#include <_common/sdebug.h>
#include "include.hpp"

extern "C" {

#include <stdlib.h>
#include <unistd.h>
#include <cstring>


cArena LibFn(new_arena)(l64 capacity) {
    if (capacity > BACKEND_ARENA_MAX_CAPACITY) 
        return (cArena){0};

    return (cArena){
        .capacity=capacity,
        .sp=0,
        .arr=(byte*)malloc(capacity)
    };
}

l64 LibFn(arena_push)(cArena* arena, l64 size, void* data) {
    if (size > BACKEND_ARENA_MAX_ENTRY_SIZE) {
        // data too large
        return BACKEND_FAIL_ARENA_TOOLARGE;
    }

    l64 initial_sp = arena->sp;
    if (initial_sp + size >= arena->capacity) {
        // out of space
        return BACKEND_FAIL_ARENA_NOSPACE;
    }

    byte_arr ba = arena->arr;
    l64 i;
    for (i = 0; i < size; i++) {
        ba[arena->sp++] = ((byte*)data)[i];
    }
    
    return initial_sp;
}

void LibFn(arena_print)(cArena* a, l64 sp_offset) {
    printf("- TODO :: backend_arena_print");
}

void LibFn(arena_clear)(cArena* a) {
    memset(a->arr, 0, a->capacity);
}

void LibFn(arena_dispose)(cArena* a) {
    if (a->arr == 0) return;
    free(a->arr);
    a->arr = 0;
}

}


void mod_memory_init() {
    PrintD("Mod::memory_init")
}

void Test_mod_memory_init() {
    PrintD("Test::memory_init")
}

namespace MAGE {
namespace Memory {



}
}
