
#ifndef MOD_ECS
#define MOD_ECS

extern "C" {
    #include "lib.h"
}


#include <unordered_map>
#include <string>
#include <vector>

#include <memory/include.hpp>

#include <_common/rmath.h>

#include <_common/interfaces.hpp>

#include <events/include.hpp>
#include <scripting/include.hpp>


namespace MAGE {
namespace ECS {

void LoadModule(lua_State* L);


class IEntity : public ICastable, public ITogglable, public IUpdatable {
public:
    virtual const char* Name() { return "IEntity"; }

    virtual bool HasComponent(const char* name) { return false; }
    virtual void* GetComponent(const char* name) { return 0; }

    virtual size_pair_t GetArenaSpan() { return {0, 0}; }

};

class ISystem {
public:
    virtual void Run(std::vector<IEntity>* entities, std::vector<IResource>* resources) {}
};



class World : public IDestroyable, public ITogglable, public IUpdatable {
public:

class IFactory {
public:
    virtual bool CanInstantiate(std::string type_name) { return false; }
    virtual ICastable* Instantiate(World* world, std::string object_type, std::string object_meta) { return (ICastable*)0; }

};
    
    static World* current;

    World() {}
    World(int cap_components) {
        this->am_components = Memory::ArenaManager(cap_components);
        ECS::World::current = this;
    }

    bool TryFactoryInstantiateEntity(std::string entity_name, std::string entity_type, std::string entity_meta);
    bool TryFactoryInstantiateResource(std::string resource_name, std::string resource_type, std::string resource_meta);

    template <class T>
    bool AddEntity(std::string name, T* entity) {
        this->entities.emplace(name, (IEntity*)entity);
        return this->entities.find(name) != this->entities.end();
    }

    static int lua_AddEntity(lua_State* L);
    bool RemoveEntity(std::string name);

    IEntity* GetEntity(std::string name);
    IEntity* GetEntity(l64 index);
    
    l64 CurrentEntityCount() { return this->entities.size(); }

    bool AddResource(std::string name, IResource* resource);
    static int lua_AddResource(lua_State* L);
    IResource* GetResource(std::string name);

    
    //void ApplySystem(ISystem* system);

    l64 GetComponentSP();
    
    template <class T>
    void* CreateComponent(T data) {
        return this->am_components.CreateObject(data);
    }

    void RegisterFactory(bool isEntity, IFactory* factory) {
        if (isEntity)
            this->entityFactories.emplace_back(factory);
        else 
            this->resourceFactories.emplace_back(factory);
    }

    void Destroy();

private:
    Memory::ArenaManager am_components;
    std::unordered_map<std::string, IEntity*> entities;
    std::unordered_map<std::string, IResource*> resources;
    std::vector<IFactory*> entityFactories;
    std::vector<IFactory*> resourceFactories;

};


class Transform : public IPrintable {
public:
    Transform() {}
    Transform(World* world) { 
        this->data.Set(world->CreateComponent(
            (cTransformData){
                .position = Vector3Zero(),
                .angles   = Vector3Zero(),
                .scale    = Vector3One()
            }
        ));
    }

    inline Vector3* position() { return &(this->data.Get()->position); }
    inline Vector3* angles()   { return &(this->data.Get()->angles  ); }
    inline Vector3* scale()    { return &(this->data.Get()->scale   ); }

    const char* ToString() {
        //cTransformData* d = this->data.Get();
        //Core::ClearPrintBuffer();
        //sprintf(Core::GetPrintBuffer(),
        //    "position: (%g, %g, %g), angles: (%g, %g, %g), scale: (%g, %g, %g)", 
        //        d->position.x, d->position.y, d->position.z,
        //        d->angles.x  , d->angles.y  , d->angles.z,
        //        d->scale.x   , d->scale.y   , d->scale.z
        //);
        
        return "transform.ToString";
        ///return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<cTransformData> data;
};

}}

#endif // !MOD_ECS
