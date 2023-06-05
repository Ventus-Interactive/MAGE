
#include <_common/sdebug.h>
#include "include.hpp"

void mod_events_init() {
    PrintD("Mod::events_init")
}

void Test_mod_events_init() {
    PrintD("Test::events_init")
}


#include <lua.hpp>



namespace MAGE {
namespace Events {



void LoadModule(lua_State* L) {
    //lua_register(L, "h_world_add_resource", World::lua_AddResource);
    //lua_register(L, "h_world_add_entity"  , World::lua_AddEntity  );

    luaL_dofile(L, "resources/scripts/core/events.lua");
    lua_setglobal(L, "events");  // expose reference
    lua_settop(L, 0);   // clear the stack
}


EventManager* EventManager::current;

void EventManager::RegisterListener(std::string event_name, IEventListener* listener) {
    this->listener_table[event_name].emplace_back(listener);
}
void EventManager::RegisterEvent(std::string event_name) {
    this->listener_table.emplace(event_name, std::vector<IEventListener*>());
}
void EventManager::PushEvent(EventData event) {
    this->events.push(event);
}
void EventManager::Update() {
    EventData event;
    int i, listeners_count;
    std::vector<IEventListener*>* listeners;

    while (this->events.empty() == false) {
        event = this->events.front(); this->events.pop();
        if (this->listener_table.find(event.name) == this->listener_table.end()) 
            continue;

        listeners = &(this->listener_table[event.name]);
        listeners_count = listeners->size();

        for (i = 0; i < listeners_count; i++) {
            listeners->at(i)->OnEvent(event.data);
        }
        
        if (event.data != 0) {
            free(event.data);
            event.data = 0;
        }
        
    }
}

}}

