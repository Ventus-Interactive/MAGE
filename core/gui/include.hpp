
#ifndef MOD_GUI
#define MOD_GUI

extern "C" {
    #include "lib.h"
}

#include <string>
#include <vector>
#include <unordered_map>

#include <_common/interfaces.hpp>

#include <events/include.hpp>
#include <ecs/include.hpp>

#include <_common/raylib.h>
#include <lua.hpp>


namespace MAGE {
namespace GUI {

void LoadModule     (lua_State* L);
void RegisterEvents (Events::EventManager* eventManager);


class IGuiComponent : public ICastable, public ITogglable, public IUpdatable {
public:
    bool active = false;
    Vector2 position;
    float rotation;
    Vector2 scale;
    void ToggleState() { this->active = !this->active; }

    virtual void Update(Camera* camera) {}
};

class Button : public IGuiComponent {
public:
    virtual void OnClick() {}
    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);
};

class ToggleButton : public Button {
public:
    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);
};

class Slider : public IGuiComponent {
public:
    virtual void OnSliderMove(float value) {}
    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);

private:
    float progress;
};

class Text : public IGuiComponent {
public:
    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);

private:
    std::string text;
};

class TextField : public Text {
public:
    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);

private: 
    std::string hint;
};

class Panel : public IGuiComponent {
public:
    Panel(Texture2D* sprite, Color tint) {
        this->sprite = sprite;
        this->tint = tint;
    }

    void ToggleState() { this->active = !this->active; }
    void Update(Camera* camera);

private:
    Texture2D* sprite;
    Color tint;
};


class IWidget : public ICastable, public IDestroyable, public ITogglable, public IUpdatable {
public:
    void ToggleState() { this->active = !this->active; }
    virtual void Update(Camera* camera) {}
    void Destroy();

private:
    bool active = false;
};


class Canvas : public IDestroyable, public ITogglable, public IUpdatable {
public:

class IFactory {
public:
    virtual bool CanInstantiate(std::string type_name) { return false; }
    virtual ICastable* Instantiate(Canvas* canvas, std::string object_type, std::string object_meta, int posX, int posY) { return (ICastable*)0; }

};

    static Canvas* current;

    Canvas(Camera* camera) { 
        this->main_camera = camera; 
        this->active = true; 
        GUI::Canvas::current = this;
    }

    void ToggleState() { this->active = !this->active; }
    void Update();

    bool TryFactoryInstantiateWidget(std::string widget_name, std::string widget_type, std::string widget_meta, int posX, int posY);
    bool TryFactoryInstantiateComponent(std::string component_name, std::string component_type, std::string component_meta, int posX, int posY);

    static int lua_AddWidget(lua_State* L);
    static int lua_AddComponent(lua_State* L);
    
    bool AddWidget(std::string name, IWidget* widget);
    bool AddComponent(std::string name, IGuiComponent* component);

    bool RegisterFactory(bool isWidget, IFactory* factory);

    void Destroy();

private:
    bool active = false;
    Camera* main_camera;
    std::unordered_map<std::string, IWidget*> widgets;
    std::unordered_map<std::string, IGuiComponent*> components;
    std::vector<IFactory*> widgetFactories;
    std::vector<IFactory*> componentFactories;
};


class GuiComponentFactory : public Canvas::IFactory {
public:
    bool CanInstantiate(std::string type_name) {
        return 
            type_name == "panel" || 
            type_name == "text";
    }

    ICastable* Instantiate(Canvas* canvas, std::string object_type, std::string object_meta, int posX, int posY);

};


}}

#endif // !MOD_GUI
