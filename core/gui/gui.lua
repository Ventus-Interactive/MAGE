
local gui = { status = 0 }

function gui.init()
    print("- Lua::Gui::Init")
    gui.status = 1
end

function gui.add_widget(widget, posX, posY)
    h_core_gui_canvas_add_widget(widget, posX, posY)
end

function gui.add_component(component, posX, posY)
    h_core_gui_canvas_add_component(component, posX, posY)
end

return gui
