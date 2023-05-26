
local ecs = { status = 0 }

function ecs.init()
    print("- Lua::Ecs::Init")
    ecs.status = 1
end

return ecs
