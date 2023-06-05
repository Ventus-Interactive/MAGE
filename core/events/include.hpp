
#ifndef MOD_EVENTS
#define MOD_EVENTS

extern "C" {
    #include "lib.h"
}

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

#include <_common/interfaces.hpp>
#include <lua.hpp>


namespace MAGE {
namespace Events {

void LoadModule(lua_State* L);


class IEventData : public ICastable {
public:
    //template <class T>
    //virtual void* Get() { return (void*)0; }
};

class IEventListener : public ICastable {
public:
    virtual void OnEvent(IEventData* data) {}
};

struct EventData {
public:
    std::string name;
    IEventData*  data;
    EventData() {
        this->name = "none";
        this->data = 0;
    }
    EventData(std::string name, IEventData* data) {
        this->name = name;
        this->data = data;
    }
};

class EventManager : public IUpdatable {
public:
    static EventManager* current;
    EventManager() { current = this; }

    void RegisterListener(std::string event_name, IEventListener* listener);
    void RegisterEvent(std::string event_name);
    void PushEvent(EventData event);

    void Update();

private:
    std::unordered_map<std::string, std::vector<IEventListener*>> listener_table;
    std::queue<EventData> events;
};

}}

#endif // !MOD_EVENTS
