

#include <_common/sdebug.h>
#include "include.hpp"

#include <cstring>

void mod_gui_init() {
    PrintD("Mod::gui_init")
}

void Test_mod_gui_init() {
    PrintD("Test::gui_init")
}

#include <rendering/include.hpp>


namespace MAGE {
namespace GUI {



void LoadModule(lua_State* L) {
    lua_register(L, "h_core_gui_canvas_add_widget", Canvas::lua_AddWidget);
    lua_register(L, "h_core_gui_canvas_add_component", Canvas::lua_AddComponent);

    luaL_dofile(L, "resources/scripts/core/gui.lua");
    lua_setglobal(L, "gui");  // expose reference
    lua_settop(L, 0);   // clear the stack
}

void RegisterEvents(Events::EventManager* eventManager) {
   // eventManager->RegisterEvent("on-collision-box");
}




void Button::Update(Camera* camera) {
    // figure out if the mouse clicked this
}

void ToggleButton::Update(Camera* camera) {
    // figure out if the mouse clicked this
}

void Slider::Update(Camera* camera) {
    // figure out if the mouse clicked the handle
    // calculate handle's position based on slider start/end
}

void Text::Update(Camera* camera) {

}

void TextField::Update(Camera* camera) {
    // figure out if the mouse clicked this
    // capture keystrokes while this is active
}

void Panel::Update(Camera* camera) {
    //if (!this->active) return;

    Texture2D tex = *(this->sprite);

    auto sourceRect = (Rectangle){
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)tex.width,
        .height = (float)tex.height
    };
    auto destRect   = (Rectangle){
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)GetScreenWidth(),
        .height = (float)GetScreenHeight()
    };

    DrawTexturePro(tex, sourceRect, destRect, this->position, this->rotation, this->tint);

}

ICastable* GuiComponentFactory::Instantiate(Canvas* canvas, std::string object_type, std::string object_meta, int posX, int posY) {
    if (object_type == "panel") {
        printf("- GuiComponent::Factory::Instantiate : panel\n");
        return new Panel(ECS::World::current->GetResource(object_meta)->As<Rendering::Texture2DResource>()->Get(), WHITE);
    }
    if (object_type == "text") {
        printf("- GuiComponent::Factory::Instantiate : no-impl : text\n");
    }

    return (ICastable*)0;
}

Canvas* Canvas::current;

bool Canvas::TryFactoryInstantiateWidget(std::string widget_name, std::string widget_type, std::string widget_meta, int posX, int posY) {
    for (auto factory : this->widgetFactories) {
        if (factory->CanInstantiate(widget_type)) {
            return this->AddWidget(widget_name, factory->Instantiate(this, widget_type, widget_meta, posX, posY)->As<IWidget>());
        }
    }
    return false;
}
bool Canvas::TryFactoryInstantiateComponent(std::string component_name, std::string component_type, std::string component_meta, int posX, int posY) {
    for (auto factory : this->componentFactories) {
        if (factory->CanInstantiate(component_type)) {
            return this->AddComponent(component_name, factory->Instantiate(this, component_type, component_meta, posX, posY)->As<IGuiComponent>());
        }
    }
    return false;
}

int Canvas::lua_AddWidget(lua_State* L) {        
    PrintD("- World::lua_AddWidget")

    // "default:pot3D:model:pot3D.obj"
    
    const char* cparams = lua_tostring(L, 1);
    const int posX = (int)lua_tonumber(L, 2);
    const int posY = (int)lua_tonumber(L, 3);

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
        std::string canvas_name = split_params[0];
        std::string widget_name = split_params[1];
        std::string widget_type = split_params[2];
        std::string widget_meta = split_params[3];

        printf("    - canvas: %s, widget: %s, type: %s, meta: %s, posX: %d, posY: %d\n", 
            canvas_name.c_str(), widget_name.c_str(), widget_type.c_str(), widget_meta.c_str(), posX, posY);
    
        Canvas* canvas = 0;
        // use canvas_name to get canvas reference
        if (canvas_name == "default") {
            canvas = Canvas::current;
        }

        if (canvas == 0) {
            printf("    - Failed to select a Canvas!\n");
            return 0;
        }

        canvas->TryFactoryInstantiateWidget(widget_name, widget_type, widget_meta, posX, posY);
    }

    return 0;

}

int Canvas::lua_AddComponent(lua_State* L) {        
    PrintD("- World::lua_AddComponent")

    // "default:pot3D:model:pot3D.obj"
    
    const char* cparams = lua_tostring(L, 1);
    const int posX = (int)lua_tonumber(L, 2);
    const int posY = (int)lua_tonumber(L, 3);

    std::string params = cparams;
    printf("    - params: %s, %ld\n", params.c_str(), params.length());


    std::vector<std::string> split_params; {
        const int bufsize = 64;
        char buf[bufsize];
        memset(buf, 0, bufsize);
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
    }
    
    if (split_params.size() == 4) {
        std::string canvas_name = split_params[0];
        std::string component_name = split_params[1];
        std::string component_type = split_params[2];
        std::string component_meta = split_params[3];
        printf("    - canvas: %s, component: %s, type: %s, meta: %s, posX: %d, posY: %d\n", 
            canvas_name.c_str(), component_name.c_str(), component_type.c_str(), component_meta.c_str(), posX, posY);
    
        Canvas* canvas = 0;
        // use canvas_name to get canvas reference
        if (canvas_name == "default") {
            canvas = Canvas::current;
        }

        if (canvas == 0) {
            printf("    - Failed to select a Canvas!\n");
            return 0;
        }

        canvas->TryFactoryInstantiateComponent(component_name, component_type, component_meta, posX, posY);
    }

    return 0;

}

bool Canvas::AddWidget(std::string name, IWidget* widget) {
    widget->ToggleState();
    this->widgets.try_emplace(name, widget);
    return true;
}
bool Canvas::AddComponent(std::string name, IGuiComponent* component) {
    component->ToggleState();
    this->components.try_emplace(name, component);
    return true;
}

bool Canvas::RegisterFactory(bool isWidget, IFactory* factory) {
    if (isWidget)
        this->widgetFactories.emplace_back(factory);
    else
        this->componentFactories.emplace_back(factory);
    return true;
}

void Canvas::Update() {
    if (!this->active) return;

    // render each widget
    for (auto& e : this->widgets) {
        if (e.second != 0)
            e.second->Update(this->main_camera);
    }

    // render each component
    for (auto& e : this->components) {
        if (e.second != 0)
            e.second->Update(this->main_camera);
    }

}

void Canvas::Destroy() {
    // drop each widget
    for (auto& e : this->widgets) {
        if (e.second != 0) {
            free(e.second);
            e.second = 0;
        }
    }
    // drop each component
    for (auto& e : this->components) {
        if (e.second != 0) {
            free(e.second);
            e.second = 0;
        }
    }

    this->widgets.clear();
    this->components.clear();

    // drop each widget factory
    for (int i = 0; i < this->widgetFactories.size(); i++) {
        free(this->widgetFactories[i]);
    }
    // drop each component factory
    for (int i = 0; i < this->componentFactories.size(); i++) {
        free(this->componentFactories[i]);
    }

    this->widgetFactories.clear();
    this->componentFactories.clear();

}


}}

