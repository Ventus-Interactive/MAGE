
#ifndef MOD_PHYSICS
#define MOD_PHYSICS

extern "C" {
    #include "lib.h"
}

#include <_common/interfaces.hpp>

#include <events/include.hpp>
#include <ecs/include.hpp>


namespace MAGE {
namespace Physics {
    
void LoadModule(lua_State* L);

/*- Global Forces (wind, water, gravity, drag, etc..)

- Collision/Trigger Callbacks
  - Entities can assign a callback or callback group
    - a callback is function signature
    - a callback group is a group of entities that share a single callback

- Box Sweep, Raycast

- Sphere Collision

- 2D/3D Particle physics

*/

void RegisterEvents(Events::EventManager* eventManager);


class BoxCollider : public IPrintable, public ITogglable, public IUpdatable {
public:

class BoxCollisionEvent : public Events::IEventData {
public:
    BoxCollisionEvent(BoxCollider* other) { this->other = other; }
    BoxCollider* other;

};

    BoxCollider() {}
    BoxCollider(ECS::World* world) { 
        this->data.Set(world->CreateComponent(
            (cBoxColliderData){
                .center = Vector3Zero(),
                .scale = Vector3One()
            }
        ));
    }

    inline Vector3* center() { return &(this->data.Get()->center); }
    inline Vector3* scale()  { return &(this->data.Get()->scale ); }

    void OnCollision(BoxCollider* other) {
        Events::EventManager::current->PushEvent(
            Events::EventData(
                "on-collision-box",
                new BoxCollisionEvent(other)
            )
        );
    }


    const char* ToString() {
        return "BoxCollider.ToString";
        //cBoxColliderData* d = this->data.Get();
        //Core::ClearPrintBuffer();
        //sprintf(Core::GetPrintBuffer(),
        //    "center: (%g, %g, %g), scale: (%g, %g, %g)", 
        //        d->center.x, d->center.y, d->center.z, 
        //        d->scale.x, d->scale.y, d->scale.z
        //);
        //
        //return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<cBoxColliderData> data;
};

class Rigidbody : public IPrintable, public ITogglable, public IUpdatable {
public:
    Rigidbody() {}
    Rigidbody(ECS::World* world) { 
        this->data.Set(world->CreateComponent(
            (cRigidbodyData){
                .mass = 1.0f,
                .drag = 0.2f,
                .velocity = Vector3Zero()
            }
        ));
    }

    inline float*   mass()     { return &(this->data.Get()->mass    ); }
    inline float*   drag()     { return &(this->data.Get()->drag    ); }
    inline Vector3* velocity() { return &(this->data.Get()->velocity); }

    const char* ToString() {
        return "Rigidbody.ToString";
        //cRigidbodyData* d = this->data.Get();
        //Core::ClearPrintBuffer();
        //sprintf(Core::GetPrintBuffer(),
        //    "mass: %g, drag: %g, velocity: (%g, %g, %g)", 
        //        d->mass, d->drag, 
        //        d->velocity.x, d->velocity.y, d->velocity.z
        //);
        //
        //return Core::GetPrintBuffer();
    }

private:
    Memory::ArenaVar<cRigidbodyData> data;
};

template<class T>
class CollisionProcedureData {
public:
    int entitiesFound;
    std::vector<ECS::Transform*>*         transforms;
    std::vector<T*>*                      colliders;
    std::vector<Physics::Rigidbody*>*     rigidbodies;
    CollisionProcedureData(
        int entitiesFound,
        std::vector<ECS::Transform*>*     transforms,
        std::vector<T*>*                  colliders,
        std::vector<Physics::Rigidbody*>* rigidbodies
    ) {
        this->entitiesFound = entitiesFound;
        this->transforms = transforms;
        this->colliders = colliders;
        this->rigidbodies = rigidbodies;
    }
};


/*
- Default Physics/Collision Systems
  - solves collision and physics forces
  - Filters through a World to find entities with physical characteristics
  - AABB Spacial Partitioning
  - Attempts to invoke callbacks when entities collide
  - Physics is calculated on Rigidbody/Collider Entities
*/
void ManageWorld(ECS::World* world);

}}

#endif // !MOD_PHYSICS
