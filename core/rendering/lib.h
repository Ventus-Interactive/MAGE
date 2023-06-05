
#ifndef LIB_RENDERING
#define LIB_RENDERING

#include <_common/raylib.h>

#include <_common/sdefs.h>
#include <_common/sdebug.h>


DS(cSpriteRendererData, 
    Texture2D* texture;
    Color tint;
)            

//DS(cAnimatorData, 
// 
//)

DS(cMeshRendererData,
   Mesh* mesh;
   Material* material;
)

DS(cModelRendererData,
   Model* model;
   Material* material;
   Color tint;
)

DS(cTextRendererData,
   //Mesh mesh;
   //Material* material;
)


void mod_rendering_init();
void Test_mod_rendering_init();

#endif // !LIB_RENDERING
