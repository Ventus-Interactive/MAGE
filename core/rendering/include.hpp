
#ifndef MOD_RENDERING
#define MOD_RENDERING

extern "C" {
    #include "lib.h"
}

#include <queue>
#include <cstring>

#include <_common/interfaces.hpp>

#include <gui/include.hpp>
#include <ecs/include.hpp>
#include <physics/include.hpp>

namespace MAGE {
namespace Rendering {


void LoadModule(lua_State* L);

//- Screen Effects

//- Procedural Meshes

//- Particle Rendering systems

//- Billboard Quads & Meshes

//- Animation Systems

//- Imposter rendering
//  - renders a single object from multiple angles and projects it onto several quads
//  - one shared object for many instances
//    - the piece of geometry can be loaded a single time and be reused


/*class SpriteRenderer : public IDestroyable, public ITogglable, public IUpdatable {
public:


private:

};

class MeshRenderer : public IDestroyable, public ITogglable, public IUpdatable {
public:


private:

};

class ModelRenderer : public IDestroyable, public ITogglable, public IUpdatable {
public:


private:

};*/

//class Animator : public IDestroyable, public ITogglable, public IUpdatable {
//public:
//
//
//private:
//
//};

//- Shader & Material Tables
//- Assignable Materials/Shaders

//- Default System:
//    - Objects are filtered out of a World
//        - the objects are rendered correctly based on attributes



enum RenderLayer {
    Static,
    Dynamic,
    Hybrid
};

DS(cLayerFlag,
    int status;
    Rendering::RenderLayer layerID;
)




class Camera3DComponent : public IPrintable, public ITogglable, public IUpdatable {
public:
    Camera3DComponent() {}
    Camera3DComponent(ECS::World* world, Camera3D camera) { 
        this->data.Set(world->CreateComponent(
            camera
        ));
    }

    inline Camera3D* camera() { return (this->data.Get()); }

    const char* ToString() {
        return "CameraComponent.ToString";
        //cMeshRendererData* d = this->data.Get();
        //Core::ClearPrintBuffer();
        //sprintf(Core::GetPrintBuffer(),
        //    "@mesh: %p, @material: %p", 
        //      d->mesh ,  d->material
        //);
        //
        //return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<Camera3D> data;
};



class MeshRenderer : public IPrintable, public ITogglable, public IUpdatable {
public:
    MeshRenderer() {}
    MeshRenderer(ECS::World* world, Mesh* mesh, Material* material) { 
        this->data.Set(world->CreateComponent(
            (cMeshRendererData){
                .mesh = mesh,
                .material = material
            }
        ));
    }

    inline Mesh*     mesh()     { return (this->data.Get()->mesh); }
    inline Material* material() { return (this->data.Get()->material); }

    const char* ToString() {
        return "MeshRenderer.ToString";
        //cMeshRendererData* d = this->data.Get();
        //Core::ClearPrintBuffer();
        //sprintf(Core::GetPrintBuffer(),
        //    "@mesh: %p, @material: %p", 
        //      d->mesh ,  d->material
        //);
        //
        //return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<cMeshRendererData> data;
};


class ModelRenderer : public IPrintable, public ITogglable, public IUpdatable {
public:
    ModelRenderer() {}
    ModelRenderer(ECS::World* world, Model* model, Material* material, Color tint) { 
        this->data.Set(world->CreateComponent(
            (cModelRendererData){
                .model = model,
                .material = material,
                .tint = tint
            }
        ));
    }

    inline Model*     model()     { return (this->data.Get()->model     ); }
    inline Material*  material()  { return &(this->model()->materials[0]); }
    inline Color*     tint()      { return &(this->data.Get()->tint     ); }

    const char* ToString() {
       return "ModelRenderer.ToString";
       // cModelRendererData* d = this->data.Get();
       // Core::ClearPrintBuffer();
       // sprintf(Core::GetPrintBuffer(),
       //     "@model: %p, @material: %p", 
       //       d->model ,  d->material
       // );  // print out 'tint'
       // 
       // return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<cModelRendererData> data;
};


template<class T>
class RenderProcedureData {
public:
    int entitiesFound;
    std::vector<ECS::Transform*>*      transforms;
    std::vector<T*>*                   renderers;
    RenderProcedureData(
        int entitiesFound,
        std::vector<ECS::Transform*>*  transforms,
        std::vector<T*>*               renderers
    ) {
        this->entitiesFound = entitiesFound;
        this->transforms = transforms;
        this->renderers = renderers;
    }
};

//#define ENTITY_ARENA_CAP 20000
#define ARENA_CAP_COMPONENT 20000
//#define RESOURCE_ARENA_CAP 20000





class RenderPipeline {
public: 
   // RenderPipeline(){}
    RenderPipeline(){
        this->default_shader = LoadShader("resources/shaders/lighting.vs", "resources/shaders/lighting.fs");
        this->default_material = LoadMaterialDefault();
        // Load basic lighting shader
        //this->default_material.shader = this->default_shader;

    }
    void Render(Camera* camera, GUI::Canvas* canvas, ECS::World* world, void (*OnRender)(Camera* camera, ECS::World*));
    void DrawMeshes3D(Camera* camera, Rendering::RenderProcedureData<Rendering::MeshRenderer>* data);
    void DrawModels3D(Camera* camera, Rendering::RenderProcedureData<Rendering::ModelRenderer>* data);
   // void SetRenderRefreshTrigger(Rendering::RenderLayer layerID) ;
    void Destroy();

    Shader default_shader;
    Material default_material;

private:
    std::queue<Rendering::RenderLayer> cmdFlags;


};



class Texture2DResource : public IResource {
public:
    Texture2DResource(Texture2D texture) : texture(texture) {}
    Texture2D* Get() { return &(this->texture); }
    void Destroy() {
        PrintD("Destroying Texture2D")
        UnloadTexture(this->texture);
    }
private:
    Texture2D texture;
};

class MeshResource : public IResource {
public:
    MeshResource(Mesh mesh) : mesh(mesh) {}
    Mesh* Get() { return &(this->mesh); }
    void Destroy() {
        PrintD("Destroying MeshResource")
        UnloadMesh(this->mesh);
    }
private:
    Mesh mesh;
};

class ModelResource : public IResource {
public:
    ModelResource(Model model) : model(model) {}
    Model* Get() { return &(this->model); }
    void Destroy() {
        PrintD("Destroying ModelResource")
        UnloadModel(this->model);
    }
private:
    Model model;
};



class StaticModel : public ECS::IEntity {
public:

    const char* Name() { return "StaticModel"; }
    
    ECS::Transform*             Transform()      { return &this->transform       ; }
    Rendering::ModelRenderer*   ModelRenderer()  { return &this->modelRenderer   ; }
    Physics::BoxCollider*       BoxCollider()    { return &this->boxCollider     ; }
    
    StaticModel(ECS::World* world, std::string model_name) {
        Model* model = world->GetResource(model_name)->As<ModelResource>()->Get();

        this->transform  = ECS::Transform(world);
        this->boxCollider = Physics::BoxCollider(world);
        this->modelRenderer = Rendering::ModelRenderer(world, model, 0, WHITE);
    }

    bool HasComponent(const char* name) {
       // printf("- %s has %s, tried: %s, === %s\n", this->Name(), "transform, model-renderer", name, (strncmp(name, "transform", 24) == 0 || strncmp(name, "model-renderer", 24) == 0) ? "valid" : "invalid");

        return 
        strncmp(name, "transform"       , 24) == 0 || 
        strncmp(name, "box-collider"    , 24) == 0 || 
        strncmp(name, "model-renderer"  , 24) == 0;
        //|| name == "box-collider";
    }
    void* GetComponent(const char* name) {
        
        if (strncmp(name, "transform"       , 24) == 0)     return this->Transform();
        if (strncmp(name, "model-renderer"  , 24) == 0)     return this->ModelRenderer();
        if (strncmp(name, "box-collider"    , 24) == 0)     return this->BoxCollider();

        //printf("IEntity(%s) does not have Component<%s>, will return (void*)0 \n", this->Name(), name);
        return 0;
    }

private:
    ECS::Transform transform;
    Physics::BoxCollider boxCollider;
    Rendering::ModelRenderer modelRenderer;

};

class ResourceFactory : public ECS::World::IFactory {
public:
    bool CanInstantiate(std::string type_name) {
        return 
            type_name == "texture2D"    ||
            type_name == "model"        || 
            type_name == "mesh";
    }

    ICastable* Instantiate(ECS::World* world, std::string object_type, std::string object_meta) {
        if (object_type == "texture2D") {
            std::string textures_path = "resources/textures/";
            textures_path.append(object_meta);
            return new Texture2DResource(LoadTexture(textures_path.c_str()));
        }
        if (object_type == "model") {
            std::string model_path = "resources/models/";
            model_path.append(object_meta);
            return new ModelResource(LoadModel(model_path.c_str()));
        }
        if (object_type == "mesh") {
            
            printf("- Rendering::Factory::Instantiate : no-impl : object_type(mesh)\n");
        }

        return (ICastable*)0;
    }

};

class EntityFactory : public ECS::World::IFactory {
public:
    bool CanInstantiate(std::string type_name) {
        return 
            type_name == "static_model" || 
            type_name == "static_mesh";
    }

    ICastable* Instantiate(ECS::World* world, std::string object_type, std::string object_meta) {
        if (object_type == "static_model") {
            auto sm = new StaticModel(world, object_meta);
            auto box = sm->BoxCollider();
            *(box->scale()) = (Vector3){
                .x = 2,
                .y = 2,
                .z = 2
            };
            box->center()->y = 0.5f;

            return sm;
        }
        if (object_type == "static_mesh") {
            printf("- Rendering::Factory::Instantiate : no-impl : static_mesh\n");
        }

        return (ICastable*)0;
    }

};

/*
class StaticBox : public Core::IEntity {
public:
    Core::Transform transform;
    Physics::BoxCollider boxCollider;
    
    StaticBox(Core::Transform transform, Physics::BoxCollider boxCollider) {
        this->transform   = transform;
        this->boxCollider = boxCollider;
    }

    bool HasComponent(const char* name) {
        return name == "transform" || name == "box-collider";
    }
    Core::Transform* GetTransform() { return &this->transform; }

private:

};

class StaticMesh : public Core::IEntity {
public:
    Core::Transform transform;
    //Physics::BoxCollider boxCollider;
    Rendering::MeshRenderer meshRenderer;

    StaticMesh(Core::Scene* scene, Mesh* mesh) {
        this->transform = Core::Transform(scene),
        this->meshRenderer = Rendering::MeshRenderer(scene, mesh, 0);
    }

    StaticMesh(Core::Transform transform, /*Physics::BoxCollider boxCollider,/ Rendering::MeshRenderer meshRenderer) {
        this->transform   = transform;
        //this->boxCollider = boxCollider;
        this->meshRenderer = meshRenderer;
    }

    const char* Name() { return "StaticMesh"; }

    bool HasComponent(const char* name) {
       // printf("- %s has %s, tried: %s, === %s\n", this->Name(), "transform, mesh-renderer", name, (strncmp(name, "transform", 24) == 0 || strncmp(name, "mesh-renderer", 24) == 0) ? "valid" : "invalid");

        return strncmp(name, "transform", 24) == 0 || strncmp(name, "mesh-renderer", 24) == 0;//|| name == "box-collider";
    }
    void* GetComponent(const char* name) {
        
        if (strncmp(name, "transform", 24) == 0)     return this->GetTransform();
        if (strncmp(name, "mesh-renderer", 24) == 0) return this->GetMeshRenderer();

        //printf("IEntity(%s) does not have Component<%s>, will return (void*)0 \n", this->Name(), name);
        return 0;
    }

    Ecs::Transform*          GetTransform()    { return &this->transform   ; }
    Rendering::MeshRenderer* GetMeshRenderer() { return &this->meshRenderer; }

private:

};*/


}}

#endif // !MOD_RENDERING
