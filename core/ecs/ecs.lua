
local ecs = { status = 0 }

function ecs.init()
    print("- Lua::ECS::Init")
    ecs.status = 1
end

function ecs.add_resource(resource)
    print("- Lua::ECS::AddResource : " .. resource)
    h_world_add_resource(resource)
end

function ecs.add_entity(entity)
    print("- Lua::ECS::AddEntity : " .. entity)
    h_world_add_entity(entity)
end

return ecs
