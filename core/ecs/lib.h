
#ifndef LIB_ECS
#define LIB_ECS


#include <_common/raylib.h>

#include <_common/sdefs.h>


DS(cTransformData,
    Vector3 position;
    Vector3 angles;
    Vector3 scale;
)

float lerp(float a, float b, float t);

void mod_ecs_init();
void Test_mod_ecs_init();

#endif // !LIB_ECS
