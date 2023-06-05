
#include <_common/sdebug.h>
#include "include.hpp"

void mod_physics_init() {
    PrintD("Mod::physics_init")
}

void Test_mod_physics_init() {
    PrintD("Test::physics_init")
}





namespace MAGE {
namespace Physics {


void LoadModule(lua_State* L) {
    //lua_register(L, "h_world_add_resource", World::lua_AddResource);
    //lua_register(L, "h_world_add_entity"  , World::lua_AddEntity  );

    luaL_dofile(L, "resources/scripts/core/physics.lua");
    lua_setglobal(L, "physics");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

void RegisterEvents(Events::EventManager* eventManager) {
    eventManager->RegisterEvent("on-collision-box");
}



void ProcessForcesAndAABB(Physics::CollisionProcedureData<Physics::BoxCollider>* data){//}, ECS::World* world) {
   
    ECS::Transform*       transformA;    ECS::Transform*       transformB;
    Physics::BoxCollider* boxColliderA;  Physics::BoxCollider* boxColliderB;
    Physics::Rigidbody*   rigidbodyA;    Physics::Rigidbody*   rigidbodyB;

    Vector3* positionA; Vector3* positionB;
    Vector3* scaleA;    Vector3* scaleB;

    BoundingBox boxA, boxB;
    float hwidthA, hheightA, hdepthA;
    float hwidthB, hheightB, hdepthB;
    float acenterx, acentery, acenterz;
    float bcenterx, bcentery, bcenterz;

    const float global_physics_gravity          = -9.2f;
    const float global_physics_movement_rate    = 0.0131f;
    const float global_physics_dampen_rate      = 0.8f;
    const float global_collision_dampen_rate    = 0.8f;
    const float global_collision_pushback_force = 0.013f;

    bool collision_found = false;

    int efound = data->entitiesFound;

    int x, y;
    for (x = 0; x < efound; x++) {
        transformA   = data->transforms->at(x);
        if (transformA == 0) continue;

        boxColliderA = data->colliders->at(x);
        rigidbodyA   = data->rigidbodies->at(x);
        if (boxColliderA == 0 && rigidbodyA == 0) continue;

        for (y = 0; y < efound; y++) {
            transformB = data->transforms->at(y);
            if (transformB == 0 || transformA == transformB) continue;
            
            rigidbodyB = data->rigidbodies->at(y);
            boxColliderB = data->colliders->at(y);
            if (boxColliderB == 0 && rigidbodyB == 0) continue;
            
            if (rigidbodyA == 0 && rigidbodyB == 0) continue;
            
            collision_found = false;

            scaleA = boxColliderA->scale(); 
            scaleB = boxColliderB->scale();  
            
            positionA = transformA->position(); 
            positionB = transformB->position();

            hwidthA  = scaleA->x / 2.0f;
            hheightA = scaleA->y / 2.0f;
            hdepthA  = scaleA->z / 2.0f;

            hwidthB  = scaleB->x / 2.0f;
            hheightB = scaleB->y / 2.0f;
            hdepthB  = scaleB->z / 2.0f;

            acenterx = boxColliderA->center()->x;
            acentery = boxColliderA->center()->y;
            acenterz = boxColliderA->center()->z;

            bcenterx = boxColliderB->center()->x;
            bcentery = boxColliderB->center()->y;
            bcenterz = boxColliderB->center()->z;


            boxA.min.x = (positionA->x - hwidthA)  + acenterx;
            boxA.min.y = (positionA->y - hheightA) + acentery;
            boxA.min.z = (positionA->z - hdepthA)  + acenterz;

            boxA.max.x = (positionA->x + hwidthA)  + acenterx;
            boxA.max.y = (positionA->y + hheightA) + acentery;
            boxA.max.z = (positionA->z + hdepthA)  + acenterz;


            boxB.min.x = (positionB->x - hwidthB)  + bcenterx;
            boxB.min.y = (positionB->y - hheightB) + bcentery;
            boxB.min.z = (positionB->z - hdepthB)  + bcenterz;

            boxB.max.x = (positionB->x + hwidthB)  + bcenterx;
            boxB.max.y = (positionB->y + hheightB) + bcentery;
            boxB.max.z = (positionB->z + hdepthB)  + bcenterz;


            if (CheckCollisionBoxes(boxA, boxB)) {
                if ((rigidbodyA != 0 && rigidbodyB == 0))
                {
                   // printf("- COLLISION :single: [ %s(collider: %d, rigidbody: %d) ... %s(collider: %d, rigidbody: %d) ]\n", 
                    //            "nameA", boxColliderA != 0, rigidbodyA != 0, "nameB", boxColliderB != 0, rigidbodyB != 0);
                    /* float x = 0;
                    if (positionB->x < positionA->x)
                        x = (positionA->x - positionB->x)
                    else
                        x = (positionB->x - positionA->x)

                    float y = 0;
                    if (positionB->y < positionA->y)
                        y = (positionA->y - positionB->y)
                    else
                        y = (positionB->y - positionA->y)

                    float z = 0;
                    if (positionB->z < positionA->z)
                        z = (positionA->z - positionB->z)
                    else
                        z = (positionB->z - positionA->z)

                    positionA->x += x * global_collision_pushback_force;
                    positionA->y += y * global_collision_pushback_force;
                    positionA->z += z * global_collision_pushback_force;*/
                  //  Vector3 dir = (Vector3){
                  //      (positionA->x - positionB->x),
                  //      (positionA->y - positionB->y),
                  //      (positionA->z - positionB->z)
                  //  };
                    //rigidbodyA->velocity()->x *
                    //rigidbodyA->velocity()->y *
                    //rigidbodyA->velocity()->z *

                    positionA->x += (positionA->x - positionB->x) * global_collision_pushback_force;
                    positionA->y += (positionA->y - positionB->y) * global_collision_pushback_force;
                    positionA->z += (positionA->z - positionB->z) * global_collision_pushback_force;
                    
                    rigidbodyA->velocity()->x = 0.0f; /// (positionA->x - positionB->x) * global_collision_pushback_force * 0.1f; ///(positionA->x - positionB->x); //lerp(rigidbodyA->velocity()->x, 0.0f, global_collision_dampen_rate);
                    rigidbodyA->velocity()->y = 0.0f; /// (positionA->y - positionB->y) * global_collision_pushback_force * 0.1f; ///(positionA->y - positionB->y); //lerp(rigidbodyA->velocity()->y, 0.0f, global_collision_dampen_rate);
                    rigidbodyA->velocity()->z = 0.0f; /// (positionA->z - positionB->z) * global_collision_pushback_force * 0.1f; ///(positionA->z - positionB->z); //lerp(rigidbodyA->velocity()->z, 0.0f, global_collision_dampen_rate);
                    
                    boxColliderB->OnCollision(boxColliderA);
                    boxColliderA->OnCollision(boxColliderB);

                    collision_found = true;
                }
            } 
            if (!collision_found) {
                //printf("- no collision [ %s ... %s ]\n", nameA, nameB);

                if (rigidbodyA != 0) {
                    boxA.min.x += rigidbodyA->velocity()->x * global_physics_movement_rate * 0.0025f;
                    boxA.min.y += rigidbodyA->velocity()->y * global_physics_movement_rate * 0.0025f;
                    boxA.min.z += rigidbodyA->velocity()->z * global_physics_movement_rate * 0.0025f;

                    boxA.max.x += rigidbodyA->velocity()->x * global_physics_movement_rate * 0.0025f;
                    boxA.max.y += rigidbodyA->velocity()->y * global_physics_movement_rate * 0.0025f;
                    boxA.max.z += rigidbodyA->velocity()->z * global_physics_movement_rate * 0.0025f;

                    if (!CheckCollisionBoxes(boxA, boxB)) {
            
                        boxA.min.x += rigidbodyA->velocity()->x * global_physics_movement_rate * 0.025f;
                        boxA.min.y += rigidbodyA->velocity()->y * global_physics_movement_rate * 0.025f;
                        boxA.min.z += rigidbodyA->velocity()->z * global_physics_movement_rate * 0.025f;

                        boxA.max.x += rigidbodyA->velocity()->x * global_physics_movement_rate * 0.025f;
                        boxA.max.y += rigidbodyA->velocity()->y * global_physics_movement_rate * 0.025f;
                        boxA.max.z += rigidbodyA->velocity()->z * global_physics_movement_rate * 0.025f;

                        if (!CheckCollisionBoxes(boxA, boxB)) {
                            transformA->position()->x += rigidbodyA->velocity()->x * global_physics_movement_rate;
                            transformA->position()->y += rigidbodyA->velocity()->y * global_physics_movement_rate;
                            transformA->position()->z += rigidbodyA->velocity()->z * global_physics_movement_rate;
                            
                            rigidbodyA->velocity()->x = lerp(rigidbodyA->velocity()->x, 0.0f, global_physics_dampen_rate);
                            rigidbodyA->velocity()->y = lerp(rigidbodyA->velocity()->y, 0.0f, global_physics_dampen_rate);
                            rigidbodyA->velocity()->z = lerp(rigidbodyA->velocity()->z, 0.0f, global_physics_dampen_rate);

                            rigidbodyA->velocity()->y += global_physics_gravity * -global_physics_gravity * ((0.6f + rigidbodyA->velocity()->y) / 1000);
    
                            if (rigidbodyA->velocity()->y < -1000) rigidbodyA->velocity()->y = -1000;
                            if (rigidbodyA->velocity()->y >  1000) rigidbodyA->velocity()->y =  1000;
                            
                            if (transformA->position()->y < boxColliderA->scale()->y/2.0f) {
                                transformA->position()->y = boxColliderA->scale()->y/2.0f;
                                rigidbodyA->velocity()->y = 0.0f;
                            }   

                        } else {
                            rigidbodyA->velocity()->x = 0.0f; ///(positionA->x - positionB->x) ;//lerp(rigidbodyA->velocity()->x, 0.0f, global_physics_dampen_rate);
                            rigidbodyA->velocity()->y = 0.0f; ///(positionA->y - positionB->y) ;//lerp(rigidbodyA->velocity()->y, 0.0f, global_physics_dampen_rate);
                            rigidbodyA->velocity()->z = 0.0f; ///(positionA->z - positionB->z) ;//lerp(rigidbodyA->velocity()->z, 0.0f, global_physics_dampen_rate);
                        }
                    } else {
                        rigidbodyA->velocity()->x = 0.0f; ///(positionA->x - positionB->x) ;//lerp(rigidbodyA->velocity()->x, 0.0f, global_physics_dampen_rate);
                        rigidbodyA->velocity()->y = 0.0f; ///(positionA->y - positionB->y) ;//lerp(rigidbodyA->velocity()->y, 0.0f, global_physics_dampen_rate);
                        rigidbodyA->velocity()->z = 0.0f; ///(positionA->z - positionB->z) ;//lerp(rigidbodyA->velocity()->z, 0.0f, global_physics_dampen_rate);
                    }
                }
            }

        }
    }

}

void ManageWorld(ECS::World* world) {
   
    static auto transforms = std::vector<ECS::Transform*>(100);
    static auto colliders = std::vector<Physics::BoxCollider*>(100);
    static auto rigidbodies = std::vector<Physics::Rigidbody*>(100);

    static auto rpd_box = Physics::CollisionProcedureData<Physics::BoxCollider>(
        0,
        &transforms,
        &colliders,
        &rigidbodies
    );
    
    int efound = 0;
    int ecount = world->CurrentEntityCount();
    //printf("- @physics.ecount: %d\n", ecount);
    for (int i = 0; i < ecount; i++) {
        auto e = world->GetEntity(i);

        transforms[i]  = 0;
        colliders[i]   = 0;
        rigidbodies[i] = 0;

        if (e->HasComponent("transform") && (e->HasComponent("rigidbody") || e->HasComponent("box-collider"))) {
            transforms[i]  = ((ECS::Transform*)e->GetComponent("transform"));

            if (e->HasComponent("rigidbody")) {
                rigidbodies[i] = ((Physics::Rigidbody*)e->GetComponent("rigidbody"));
            }

            if (e->HasComponent("box-collider")) {
                colliders[i]   = ((Physics::BoxCollider*)e->GetComponent("box-collider"));
            }

            // PrintD("found valid physics-object (transform, rigidbody, box-collider)")
            efound++;
        }
    }
    //printf("- @physics.transforms.size(): %ld\n", transforms.size());

    rpd_box.entitiesFound = efound;

    ProcessForcesAndAABB(&rpd_box);

}


}}




