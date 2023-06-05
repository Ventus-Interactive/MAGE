
#include <_common/sdebug.h>
#include "include.hpp"

void mod_rendering_init() {
    PrintD("Mod::rendering_init")
}

void Test_mod_rendering_init() {
    PrintD("Test::rendering_init")
}



namespace MAGE {
namespace Rendering {



void LoadModule(lua_State* L) {
    //lua_register(L, "h_world_add_resource", World::lua_AddResource);
    //lua_register(L, "h_world_add_entity"  , World::lua_AddEntity  );

    luaL_dofile(L, "resources/scripts/core/rendering.lua");
    lua_setglobal(L, "rendering");  // expose reference
    lua_settop(L, 0);   // clear the stack
}


void RenderPipeline::DrawMeshes3D(Camera* camera, Rendering::RenderProcedureData<Rendering::MeshRenderer>* data) { 
    
    //ECS::Transform* transform;
    //Rendering::MeshRenderer* meshRenderer;
    //
    //Vector3* position;
    //Vector3* scale;
//
    //BeginMode3D(*camera);
//
    //    for(auto& iter : *(data->transforms)){//} = data->transforms->begin(); iter != data->transforms->end(); ++iter) {
    // 
    //        const char* name = iter.first;
    //        if (name == "") {
    //            printf("- invalid name:\n");
    //            continue;
    //        }
//
//
    //        transform = data->transforms->at(name);
    //        position  = transform->position();
    //        scale     = transform->scale();
//
    //        meshRenderer = data->renderers->at(name);
//
    //        //printf("- drawing mesh!\n");
    //        DrawMesh(
    //            *(meshRenderer->mesh()), 
    //            this->default_material, 
    //            MatrixMultiply(
    //                MatrixIdentity(), 
    //                MatrixMultiply(
    //                    MatrixScale(scale->x, scale->y, scale->z), 
    //                    MatrixTranslate(position->x, position->y, position->z)
    //                )
    //            )
    //        );
    //    }
//
    //EndMode3D();
}

void RenderPipeline::DrawModels3D(Camera* camera, Rendering::RenderProcedureData<Rendering::ModelRenderer>* data) { 
    
    ECS::Transform* transform;
    Rendering::ModelRenderer* modelRenderer;
    
    Vector3* position;
    Vector3* scale;
    Color* tint;

    int render_counter = 0;
    int efound = data->entitiesFound;

    BeginMode3D(*camera);

        for (int i = 0; i < efound; i++) { 
            transform = data->transforms->at(i);
            modelRenderer = data->renderers->at(i);
            if (transform == 0 || modelRenderer == 0) {
                //PrintD("bad renderer")
                continue;
            }

            position  = transform->position();
            scale     = transform->scale();

            tint = modelRenderer->tint();

            if (modelRenderer->model() != 0) {
                DrawModel(*(modelRenderer->model()), *position, scale->y, *tint);
                render_counter++;
            }
            else printf("- no model to draw!\n");
        }

    EndMode3D();
    //printf("- missed drawing: %d\n", efound - render_counter);

}


void RenderPipeline::Destroy() {
    UnloadShader(this->default_shader);
}

void RenderPipeline::Render(Camera* camera, GUI::Canvas* canvas, ECS::World* world, void (*OnRender)(Camera* camera, ECS::World*)) {
  
    static auto transforms = std::vector<ECS::Transform*>(100);
    static auto renderers = std::vector<Rendering::ModelRenderer*>(100);

    static auto rpd_model = Rendering::RenderProcedureData<Rendering::ModelRenderer>(
        0,
        &transforms,
        &renderers
    );



    BeginDrawing();
        
        ClearBackground(SKYBLUE);

        this->DrawModels3D(camera, &rpd_model);

        OnRender(camera, world);

        BeginMode3D(*camera);

        int efound = 0;
        int ecount = world->CurrentEntityCount();
        for (int i = 0; i < ecount; i++) {
            auto e = world->GetEntity(i);

            transforms[i] = 0;
            renderers[i] = 0;

            if (e->HasComponent("transform")) {
                if (e->HasComponent("model-renderer")) {
                    transforms[i] = (ECS::Transform*)(e->GetComponent("transform"));
                    renderers[i] = (Rendering::ModelRenderer*)(e->GetComponent("model-renderer"));
                    //PrintD("found valid renderable (transform, model-renderer)")
                    efound++;
                }
                
                if (transforms[i] != 0) { /// debug all transforms/box-colliders
                    Color color = RED;
                    color.a = 150;

                    auto pos = *(transforms[i]->position());

                    if (e->HasComponent("box-collider")) {
                        auto box = (Physics::BoxCollider*)(e->GetComponent("box-collider"));
                        auto box_center = box->center();
                        
                        pos.x += box_center->x;///2.0f;
                        pos.y += box_center->y;///2.0f;
                        pos.z += box_center->z;///2.0f;
                        
                        auto box_scale = *(box->scale());
                        box_scale.x /= 2.0f;
                        box_scale.y /= 2.0f;
                        box_scale.z /= 2.0f;

                        DrawCubeWiresV(pos, box_scale, color);
                    } else {
                        /// only a transform
                        float radius = Vector3Length(*(transforms[i]->scale()));
                        DrawSphereWires(pos, radius, 4, 4, color);
                    }

                }

            }

        }

        rpd_model.entitiesFound = efound;


            DrawGrid(100, 100);

        EndMode3D();

        //this->DrawSprites3D(camera, &rpd_sprite);
        //this->DrawMeshes3D(camera, &rpd_mesh);
        

        canvas->Update();

    EndDrawing();

}

}}

