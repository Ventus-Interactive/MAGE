
#ifndef MOD_MEMORY
#define MOD_MEMORY

extern "C" {
    #include <stdlib.h>
    #include "lib.h"
} 


#include <_common/interfaces.hpp>


namespace MAGE {
namespace Memory {

template<class T>
class ArenaVar {
public:
    ArenaVar() {}
    ArenaVar(void* data_loc) {
        this->data_loc = (T*)data_loc;
    }

    T* Get() {
        return this->data_loc;
    }
    T Unwrap() {
        return *this->data_loc;
    }
    void Set(void* data_loc) {
        this->data_loc = (T*)data_loc;
    }

private:
    T* data_loc;
};

class ArenaManager : public IDestroyable {
public:
    ArenaManager();
    ArenaManager(l64 arena_capacity);
    void        Clear();
    void        Destroy();

    cArena*     GetArena();
    //void*       CreateObject(u32 size, void* obj);

    template<class T> 
    void* CreateObject(T obj) {
        /// todo: custom create 
        ///     check memory regions
        ///         if the new object is too big, do a normal arena push
        ///         if the object fits, resize the region to start after this new object

        T _obj = obj;
        l64 size = sizeof(obj);
        l64 sp = LibFn(arena_push)(&this->arena, size, &_obj);
        
        if (Backend_FailArena(sp)) 
            return (void*)0;

        void* loc = &this->arena.arr[sp];
        printf("- MEME::CreateObject : (bytes):%lu, (ArenaAddr):%p, (SP): %lu\n", size, loc, sp);

        return loc;
    }

private:
    cArena arena;
};

}
}

#endif // !MOD_MEMORY
