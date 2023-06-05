
#ifndef LIB_PHYSICS
#define LIB_PHYSICS


#include <_common/stypes.h>
#include <_common/sdefs.h>
#include <raylib.h>


DS(cBoxColliderData,
    Vector3 center;
    Vector3 scale;
)
DS(cRigidbodyData,
    float mass;
    float drag;
    Vector3 velocity;
)


void mod_physics_init();
void Test_mod_physics_init();

#endif // !LIB_PHYSICS
