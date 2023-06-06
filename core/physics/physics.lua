
local physics = { status = 0 }

function physics.init()
    print("- Lua::Physics::Init")
    physics.status = 1
end

function physics.box_set_scale(box, scaleX, scaleY, scaleZ)
    h_core_physics_world_box_set_scale(box, scaleX, scaleY, scaleZ)
end
function physics.box_set_center(box, scaleX, scaleY, scaleZ)
    h_core_physics_world_box_set_center(box, scaleX, scaleY, scaleZ)
end

return physics
