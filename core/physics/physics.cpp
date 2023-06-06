
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
    lua_register(L, "h_core_physics_world_box_set_scale", lua_WorldBoxSetScale);
    lua_register(L, "h_core_physics_world_box_set_center", lua_WorldBoxSetCenter);
    lua_register(L, "h_core_physics_world_box_set_istrigger", lua_WorldBoxSetIsTrigger);

    luaL_dofile(L, "resources/scripts/core/physics.lua");
    lua_setglobal(L, "physics");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

int lua_WorldBoxSetScale(lua_State* L) {
    std::string entity_name = lua_tostring(L, 1);
    auto entity = ECS::World::current->GetEntity(entity_name);

    Physics::BoxCollider* box = (Physics::BoxCollider*)entity->GetComponent("box-collider");
    if (box == 0) return 0;
    
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    *(box->scale()) = (Vector3){x, y ,z};

    return 0;
}
int lua_WorldBoxSetCenter(lua_State* L) {
    std::string entity_name = lua_tostring(L, 1);
    auto entity = ECS::World::current->GetEntity(entity_name);

    Physics::BoxCollider* box = (Physics::BoxCollider*)entity->GetComponent("box-collider");
    if (box == 0) return 0;
    
    float x = (float)lua_tonumber(L, 2);
    float y = (float)lua_tonumber(L, 3);
    float z = (float)lua_tonumber(L, 4);

    *(box->center()) = (Vector3){x, y ,z};

    return 0;
}
int lua_WorldBoxSetIsTrigger(lua_State* L) {
    std::string entity_name = lua_tostring(L, 1);
    auto entity = ECS::World::current->GetEntity(entity_name);

    Physics::BoxCollider* box = (Physics::BoxCollider*)entity->GetComponent("box-collider");
    if (box == 0) return 0;
    
    bool is_trigger = (bool)lua_toboolean(L, 2);

    *(box->is_trigger()) = is_trigger;

    return 0;
}




void RegisterEvents(Events::EventManager* eventManager) {
    eventManager->RegisterEvent("on-collision-box");
    eventManager->RegisterEvent("on-trigger-box");
}



void ProcessForcesAndAABB(Physics::CollisionProcedureData<Physics::BoxCollider>* data){//}, ECS::World* world) {
    float timedelta = GetFrameTime();

    ECS::Transform*       transformA;    ECS::Transform*       transformB;
    Physics::BoxCollider* boxColliderA;  Physics::BoxCollider* boxColliderB;
    Physics::Rigidbody*   rigidbodyA;    Physics::Rigidbody*   rigidbodyB;

    Vector3* positionA; Vector3* positionB;
    Vector3* scaleA;    Vector3* scaleB;

    BoundingBox boxA, boxB, box_pred;
    float hwidthA, hheightA, hdepthA;
    float hwidthB, hheightB, hdepthB;
    float acenterx, acentery, acenterz;
    float bcenterx, bcentery, bcenterz;

    const float global_physics_gravity          = -9.2f;
    const float global_physics_movement_rate    = 1.0f;
    const float global_physics_dampen_rate      = 0.085f;
    //const float global_physics_sweep_dampen_rate= 1.0f;
    //const float global_collision_dampen_rate    = 1.0f;
    const float global_collision_pushback_force = 1.0f;

    bool collision_found = false;
    bool sweep_found = false;

    int efound = data->entitiesFound;

    int sweeper = 0;
    const int maxSweepSteps = 100;
    const float sweepRange = 21.5f;
    const float sweepStep = sweepRange / (float)maxSweepSteps; 


    int x, y;
    for (x = 0; x < efound; x++) {
        transformA   = data->transforms->at(x);
        if (transformA == 0) continue;

        boxColliderA = data->colliders->at(x);
        rigidbodyA   = data->rigidbodies->at(x);
        if (boxColliderA == 0 || rigidbodyA == 0) continue;

        scaleA = boxColliderA->scale(); 
        positionA = transformA->position(); 

        hwidthA  = scaleA->x / 4.0f;
        hheightA = scaleA->y / 4.0f;
        hdepthA  = scaleA->z / 4.0f;

        acenterx = boxColliderA->center()->x;
        acentery = boxColliderA->center()->y;
        acenterz = boxColliderA->center()->z;

        sweep_found = false;

        for (y = 0; y < efound; y++) {
            transformB = data->transforms->at(y);
            if (x == y || transformB == 0 || transformB == transformA) continue;
            
            rigidbodyB   = data->rigidbodies->at(y);
            boxColliderB = data->colliders->at(y);
            if (boxColliderB == 0) continue;
            
            scaleB    = boxColliderB->scale();     
            positionB = transformB->position();

            hwidthB  = scaleB->x / 4.0f;
            hheightB = scaleB->y / 4.0f;
            hdepthB  = scaleB->z / 4.0f;

            bcenterx = boxColliderB->center()->x;
            bcentery = boxColliderB->center()->y;
            bcenterz = boxColliderB->center()->z;

            boxB.min.x = (positionB->x + bcenterx) - hwidthB;
            boxB.min.y = (positionB->y + bcentery) - hheightB;
            boxB.min.z = (positionB->z + bcenterz) - hdepthB;

            boxB.max.x = (positionB->x + bcenterx) + hwidthB;
            boxB.max.y = (positionB->y + bcentery) + hheightB;
            boxB.max.z = (positionB->z + bcenterz) + hdepthB;

            box_pred.min.x = boxA.min.x = (positionA->x + acenterx) - hwidthA;
            box_pred.min.y = boxA.min.y = (positionA->y + acentery) - hheightA;
            box_pred.min.z = boxA.min.z = (positionA->z + acenterz) - hdepthA;

            box_pred.max.x = boxA.max.x = (positionA->x + acenterx) + hwidthA;
            box_pred.max.y = boxA.max.y = (positionA->y + acentery) + hheightA;
            box_pred.max.z = boxA.max.z = (positionA->z + acenterz) + hdepthA;


            collision_found = CheckCollisionBoxes(boxA, boxB);

            if (collision_found && *(boxColliderB->is_trigger())) {
                boxColliderA->OnCollision(data->owners->at(y), boxColliderB);
                collision_found = false;
               // if (rigidbodyB == 0)
                //    boxColliderB->OnCollision(data->owners->at(x), boxColliderA);
                continue;
            }


            sweep_found = false;

            /// Box Sweep
            //if (!collision_found) {
                Vector3 vel = Vector3Normalize(
                    *(rigidbodyA->velocity()));
                float velX = vel.x * .31f;
                float velY = vel.y * .31f;
                float velZ = vel.z * .31f;

                for (sweeper = 0; sweeper < maxSweepSteps; sweeper++) {
                    box_pred.min.x = boxA.min.x * velX * sweeper * sweepStep;
                    box_pred.min.y = boxA.min.y * velY * sweeper * sweepStep;
                    box_pred.min.z = boxA.min.z * velZ * sweeper * sweepStep;

                    box_pred.max.x = boxA.max.x * velX * sweeper * sweepStep;
                    box_pred.max.y = boxA.max.y * velY * sweeper * sweepStep;
                    box_pred.max.z = boxA.max.z * velZ * sweeper * sweepStep;

                    if (CheckCollisionBoxes(box_pred, boxB)) {
                        sweep_found = true;
                        break;
                    }
                }
           // }

            if (collision_found) {
                printf("- collision found!\n");
                
                //float dist = Vector3DistanceSqr(*positionA, *positionB);
                //if (dist > scaleB->)
                 //   dist = 1000.0f;

                float push_force_x = 0.5f * (sweeper / sweepRange);//((scaleB->x - dist) / scaleB->x) * 10.0f;
                float push_force_y = 0.5f * (sweeper / sweepRange);//((scaleB->y - dist) / scaleB->y) * 10.0f;
                float push_force_z = 0.5f * (sweeper / sweepRange);//((scaleB->z - dist) / scaleB->z) * 10.0f;

                // hit something, adjust velocity
                rigidbodyA->velocity()->x = 0.0f; // *= push_force_x * timedelta;
                rigidbodyA->velocity()->y = 0.0f; // *= push_force_y * timedelta;
                rigidbodyA->velocity()->z = 0.0f; // *= push_force_z * timedelta;

                Vector3 push_dir = Vector3Normalize((Vector3){
                    .x = (positionA->x - positionB->x),
                    .y = (positionA->y - positionB->y),
                    .z = (positionA->z - positionB->z)
                });
                
                positionA->x += push_dir.x * global_collision_pushback_force * push_force_x * timedelta * 0.5f;
                positionA->y += push_dir.y * global_collision_pushback_force * push_force_y * timedelta * 0.5f;
                positionA->z += push_dir.z * global_collision_pushback_force * push_force_z * timedelta * 0.5f;

                boxColliderA->OnCollision(data->owners->at(y), boxColliderB);
                //if (rigidbodyB == 0)
                  //  boxColliderB->OnCollision(data->owners->at(x), boxColliderA);

            } if (sweep_found) {
                printf("- sweep found!\n");
                // dampen velocity
                //rigidbodyA->velocity()->x *= 0.95f * timedelta;///*(positionA->x - positionB->x) * global_collision_pushback_force * 0.1f;*/ lerp(rigidbodyA->velocity()->x, 0.0f, global_physics_sweep_dampen_rate);
                //rigidbodyA->velocity()->y *= 0.95f * timedelta;///*(positionA->y - positionB->y) * global_collision_pushback_force * 0.1f;*/ lerp(rigidbodyA->velocity()->y, 0.0f, global_physics_sweep_dampen_rate);
                //rigidbodyA->velocity()->z *= 0.95f * timedelta;///*(positionA->z - positionB->z) * global_collision_pushback_force * 0.1f;*/ lerp(rigidbodyA->velocity()->z, 0.0f, global_physics_sweep_dampen_rate);
                //positionA->x += ((positionA->x - positionB->x) * global_collision_pushback_force) * 0.01f;
                //positionA->y += ((positionA->y - positionB->y) * global_collision_pushback_force) * 0.01f;
                //positionA->z += ((positionA->z - positionB->z) * global_collision_pushback_force) * 0.01f;
            } 
            /*if (!collision_found && !sweep_found) {
                Vector3 vel_dir = Vector3Normalize((Vector3){
                    .x = rigidbodyA->velocity()->x,
                    .y = rigidbodyA->velocity()->y,
                    .z = rigidbodyA->velocity()->z
                });
                
                // apply velocity to transform
                positionA->x += vel_dir.x * global_physics_movement_rate * timedelta;
                positionA->y += vel_dir.y * global_physics_movement_rate * timedelta;
                positionA->z += vel_dir.z * global_physics_movement_rate * timedelta;
                
                // dampen velocity
                if (rigidbodyA->velocity()->x != 0.0f)
                    rigidbodyA->velocity()->x *= global_physics_dampen_rate * timedelta;
                if (rigidbodyA->velocity()->y != 0.0f)
                    rigidbodyA->velocity()->y *= global_physics_dampen_rate * timedelta;
                if (rigidbodyA->velocity()->z != 0.0f)
                    rigidbodyA->velocity()->z *= global_physics_dampen_rate * timedelta;

                // add gravity to velocity
                rigidbodyA->velocity()->y += timedelta * global_physics_gravity * -global_physics_gravity;//  * ((0.6f + rigidbodyA->velocity()->y) / 1000

                // simple vertical bounds
                if (rigidbodyA->velocity()->y < -1000) 
                    rigidbodyA->velocity()->y = -1000;
                if (rigidbodyA->velocity()->y >  1000) 
                    rigidbodyA->velocity()->y =  1000;
                
                // ground height check
                if (positionA->y < boxColliderA->scale()->y/2.0f) {
                    positionA->y = boxColliderA->scale()->y/2.0f;
                    rigidbodyA->velocity()->y = 0.0f;
                }   

            }*/

        }
        if (!collision_found) {//}

        Vector3* velp = rigidbodyA->velocity();
        const float drag = 0.1f;

        Vector3 vel_dir = //Vector3Normalize(
            *(velp);//);
        
        // apply velocity to transform
        positionA->x += vel_dir.x * global_physics_movement_rate * timedelta;
        positionA->y += vel_dir.y * global_physics_movement_rate * timedelta;
        positionA->z += vel_dir.z * global_physics_movement_rate * timedelta;
        

        //rigidbodyA->velocity()->x *= (1.0f - drag) * global_physics_dampen_rate * timedelta*100.0f; // lerp(rigidbodyA->velocity()->x, 0, (1.0f - drag) * global_physics_dampen_rate * timedelta);
        //rigidbodyA->velocity()->y *= (1.0f - drag) * global_physics_dampen_rate * timedelta*100.0f; // lerp(rigidbodyA->velocity()->y, 0, (1.0f - drag) * global_physics_dampen_rate * timedelta);
        //rigidbodyA->velocity()->z *= (1.0f - drag) * global_physics_dampen_rate * timedelta*100.0f; // lerp(rigidbodyA->velocity()->z, 0, (1.0f - drag) * global_physics_dampen_rate * timedelta);

        // add gravity to velocity
        velp->y += timedelta * global_physics_gravity * -global_physics_gravity;//  * ((0.6f + rigidbodyA->velocity()->y) / 1000

        // dampen velocity
        float damp = global_physics_dampen_rate * timedelta;
        if (velp->x < 0.0f) {
            // add
            velp->x += damp;
        } else if (velp->x > 0.0f) {
            // sub
            velp->x -= damp;
        }  

        if (velp->y < 0.0f) {
            // add
            velp->y += damp;
        } else if (velp->y > 0.0f) {
            // sub
            velp->y -= damp;
        }  

        if (velp->z < 0.0f) {
            // add
            velp->z += damp;
        } else if (velp->z > 0.0f) {
            // sub
            velp->z -= damp;
        }  
        

        // simple vertical bounds
        if (velp->y < -1000) 
            velp->y = -1000;
        if (velp->y >  1000) 
            velp->y =  1000;
        
        // ground height check
        if (positionA->y < boxColliderA->scale()->y/2.0f) {
            positionA->y = boxColliderA->scale()->y/2.0f;
            velp->y = 0.0f;
        }   
    }


    }

}

void ManageWorld(ECS::World* world) {
   
    static auto owners = std::vector<std::string>(100);
    static auto transforms = std::vector<ECS::Transform*>(100);
    static auto colliders = std::vector<Physics::BoxCollider*>(100);
    static auto rigidbodies = std::vector<Physics::Rigidbody*>(100);

    static auto rpd_box = Physics::CollisionProcedureData<Physics::BoxCollider>(
        0,
        &owners,
        &transforms,
        &colliders,
        &rigidbodies
    );
    
    int efound = 0;
    int ecount = world->CurrentEntityCount();
    //printf("- @physics.ecount: %d\n", ecount);
    for (int i = 0; i < ecount; i++) {
        auto e = world->GetEntity(i);

        owners[i]      = "";
        transforms[i]  = 0;
        colliders[i]   = 0;
        rigidbodies[i] = 0;

        if (e->HasComponent("transform") && (e->HasComponent("rigidbody") || e->HasComponent("box-collider"))) {
            owners[i]       = e->Name();
            transforms[i]   = ((ECS::Transform*)e->GetComponent("transform"));

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




