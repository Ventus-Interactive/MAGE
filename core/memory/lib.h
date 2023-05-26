
#ifndef LIB_MEMORY
#define LIB_MEMORY


void mod_memory_init();
void Test_mod_memory_init();

#include <.common/stypes.h>


#define BACKEND_ARENA_MAX_ENTRY_SIZE 16000
#define BACKEND_ARENA_MAX_CAPACITY 10000000000
#define BACKEND_FAIL_ARENA_NOSPACE 10000000001
#define BACKEND_FAIL_ARENA_TOOLARGE 10000000002
#define Backend_FailArena(val) val == BACKEND_FAIL_ARENA_NOSPACE || val == BACKEND_FAIL_ARENA_TOOLARGE

DS(cArena,
    l64 capacity;
    l64 sp;
    byte_arr arr;
)

DS(cLinkedArena,
    cArena current;
    cLinkedArena* next;
)

cArena  LibFn(new_arena     )(l64 capacity);
l64     LibFn(arena_push    )(cArena* a, l64 size, void* data);
void    LibFn(arena_print   )(cArena* a, l64 sp_offset);
void    LibFn(arena_clear   )(cArena* a);
void    LibFn(arena_dispose )(cArena* a);

#endif // !LIB_MEMORY
