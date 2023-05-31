
#ifndef MOD_MEMORY
#define MOD_MEMORY

extern "C" {
    #include <stdlib.h>
    #include "lib.h"
}


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



}
}

#endif // !MOD_MEMORY
