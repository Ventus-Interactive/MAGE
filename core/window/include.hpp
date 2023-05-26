
#ifndef MOD_NAME
#define MOD_NAME

extern "C" {
    #include "lib.h"
}

#include <raylib.h>

namespace MAGE {
namespace Window {

class Application {
public:
    void Init(const char* name, int width, int height);
    static void OnExit();
    Camera* GetCamera() { return &this->camera; }

private:
    Camera camera;
};

}
}


#endif // !MOD_NAME
