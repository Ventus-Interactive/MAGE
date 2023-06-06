
#include <_common/sdebug.h>
#include "include.hpp"

#include <vector>
#include <cstring>
#include <string>

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
} 

void mod_ecs_init() {
    PrintD("Mod::ecs_init")
}

void Test_mod_ecs_init() {
    PrintD("Test::ecs_init")
}


namespace MAGE {
namespace ECS {

World* World::current;

void LoadModule(lua_State* L) {
    lua_register(L, "h_core_ecs_world_add_resource", lua_WorldAddResource);
    lua_register(L, "h_core_ecs_world_add_entity"  , lua_WorldAddEntity  );

    lua_register(L, "h_core_ecs_world_transform_set_position", lua_WorldTransformSetPosition);
    lua_register(L, "h_core_ecs_world_transform_set_scale", lua_WorldTransformSetScale);

    luaL_dofile(L, "resources/scripts/core/ecs.lua");
    lua_setglobal(L, "ecs");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

//void Scene::Init(int bytes) {
 //   this->arena = LibFn(new_arena)(bytes);
//}
//void Scene::Destroy() {
 //   LibFn(arena_dispose)(&this->arena);
//}

//Memory::ArenaManager* World::PushManager(int capacity) {
//    this->blocks.emplace_back(Memory::ArenaManager(capacity));
 //   return &this->blocks[this->blocks.size()-1];
//}

bool World::TryFactoryInstantiateResource(std::string resource_name, std::string resource_type, std::string resource_meta) {
    for (auto factory : this->resourceFactories) {
        if (factory->CanInstantiate(resource_type)) {
            return this->AddResource(resource_name, factory->Instantiate(this, resource_type, resource_meta)->As<IResource>());
        }
    }
    return false;
}

bool World::TryFactoryInstantiateEntity(std::string entity_name, std::string entity_type, std::string entity_meta) {
    for (auto factory : this->entityFactories) {
        if (factory->CanInstantiate(entity_type)) {
            auto ent = factory->Instantiate(this, entity_type, entity_meta)->As<IEntity>();
            ent->SetName(entity_name);
            return this->AddEntity(entity_name, ent);
        }
    }
    return false;
}


void World::Destroy() {
    if (!this->entities.empty()) 
    for (auto iter = this->entities.begin(); iter != this->entities.end(); ++iter) {
        //iter->second->Destroy();
        free(iter->second);
    }
    if (!this->resources.empty()) 
    for (auto iter = this->resources.begin(); iter != this->resources.end(); ++iter) {
        iter->second->Destroy();
        free(iter->second);
    }

    for (int i = 0; i < this->entityFactories.size(); i++) {
        free(this->entityFactories[i]);

    }
    for (int i = 0; i < this->resourceFactories.size(); i++) {
        free(this->resourceFactories[i]);
    }

    this->am_components.Destroy();
}

l64 World::GetComponentSP() {
    return this->am_components.GetArena()->sp;
}


int lua_WorldTransformSetPosition(lua_State* L) {
    std::string entity_name = lua_tostring(L, 1);
    auto entity = ECS::World::current->GetEntity(entity_name);
    if (entity->HasComponent("transform") == false) 
        return 0;

    ECS::Transform* transform = (ECS::Transform*)entity->GetComponent("transform");
    
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    *(transform->position()) = (Vector3){x, y ,z};

    return 0;
}
int lua_WorldTransformSetScale(lua_State* L) {
    std::string entity_name = lua_tostring(L, 1);
    auto entity = ECS::World::current->GetEntity(entity_name);
    if (entity->HasComponent("transform") == false) 
        return 0;

    ECS::Transform* transform = (ECS::Transform*)entity->GetComponent("transform");
    
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    *(transform->scale()) = (Vector3){x, y ,z};

    return 0;
}

int lua_WorldAddEntity(lua_State* L) {
    PrintD("- World::lua_AddEntity")
    
    
    // "default:potty:static_model:pot3D"
    const char* cparams = lua_tostring(L, 1);
    std::string params = cparams;
    printf("    - params: %s, %ld\n", params.c_str(), params.length());


    const int bufsize = 64;
    char buf[bufsize];
    memset(buf, 0, bufsize);
    std::vector<std::string> split_params;
    int c = 0, i, len = params.length();
    for (i = 0; i < len; i++) {
        if (cparams[i] == ':') {
            std::string s = buf;
            split_params.emplace_back(s);
            c = 0;
            memset(buf, 0, bufsize);
            continue;
        }

        buf[c++] = cparams[i];

        if (i+1 == len) {
            std::string s = buf;
            split_params.emplace_back(s);
            break;
        }
    }
    
    if (split_params.size() == 4) {
        std::string world_name  = split_params[0];
        std::string entity_name = split_params[1];
        std::string entity_type = split_params[2];
        std::string entity_meta = split_params[3];
        printf("    - world: %s, entity: %s, type: %s, meta: %s\n", world_name.c_str(), entity_name.c_str(), entity_type.c_str(), entity_meta.c_str());
    
        World* world = 0;
        // use world_name to get world reference
        if (world_name == "default") {
            world = World::current;
        }

        if (world == 0) {
            printf("    - Failed to select a World!\n");
            return 0;
        }

        // give entity_type to the factory chain, to create instance of type, using entity_meta
        // create entity with name and new instance
        if (!world->TryFactoryInstantiateEntity(entity_name, entity_type, entity_meta)) {
            printf("    - Failed TryFactoryInstantiateEntity\n");
        }
    }


    return 0;
}
int lua_WorldAddResource(lua_State* L) {
    PrintD("- World::lua_AddResource")

    // "default:pot3D:model:pot3D.obj"
    
    const char* cparams = lua_tostring(L, 1);
    std::string params = cparams;
    printf("    - params: %s, %ld\n", params.c_str(), params.length());


    const int bufsize = 64;
    char buf[bufsize];
    memset(buf, 0, bufsize);
    std::vector<std::string> split_params;
    int c = 0, i, len = params.length();
    for (i = 0; i < len; i++) {
        if (cparams[i] == ':') {
            std::string s = buf;
            split_params.emplace_back(s);
            c = 0;
            memset(buf, 0, bufsize);
            continue;
        }

        buf[c++] = cparams[i];

        if (i+1 == len) {
            std::string s = buf;
            split_params.emplace_back(s);
            break;
        }
    }
    
    if (split_params.size() == 4) {
        std::string world_name    = split_params[0];
        std::string resource_name = split_params[1];
        std::string resource_type = split_params[2];
        std::string resource_meta = split_params[3];
        printf("    - world: %s, resource: %s, type: %s, meta: %s\n", 
            world_name.c_str(), resource_name.c_str(), resource_type.c_str(), resource_meta.c_str());
    
        World* world = 0;
        // use world_name to get world reference
        if (world_name == "default") {
            world = World::current;
        }

        if (world == 0) {
            printf("    - Failed to select a World!\n");
            return 0;
        }

        // give resource_type to the factory chain, to create instance of type, using resource_meta
        // create resource with name and new instance
        if (!world->TryFactoryInstantiateResource(resource_name, resource_type, resource_meta)) {
            printf("    - Failed TryFactoryInstantiateResource\n");
        }
    }

    return 0;
}


bool World::RemoveEntity(std::string name) {
    this->entities.erase(name);
    return this->entities.find(name) == this->entities.end();
}
IEntity* World::GetEntity(std::string name) {
    return this->entities[name];
}
IEntity* World::GetEntity(l64 index) {
    auto a = this->entities.begin();
    for (int i = 0; i < index; i++) {
        ++a;
    }
    return a->second;
}

bool World::AddResource(std::string name, IResource* resource) {
    this->resources.emplace(name, resource);
    return this->resources.find(name) != this->resources.end();
}

IResource* World::GetResource(std::string name) {
    return this->resources[name];
}

}}

