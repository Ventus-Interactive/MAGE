
#ifndef MOD_INPUT
#define MOD_INPUT

extern "C" {
    #include "lib.h"
}


#include <unordered_map>
#include <vector>



namespace MAGE {
namespace Input {

enum InputState {
    Input_State_Down = 1,
    Input_State_Pressed,
    Input_State_Released,
    Input_State_Up
};

DT(void (cb*)(InputState), callback_action_t)

DS(ActionMapData,
    InputState input_actions[4];
    std::vector<callback_action_t> actions;
)

DT(std::unordered_map<int, ActionMapData>, input_settings_t)


class ActionManager : public ITogglable, public IUpdatable {
public:
    input_settings_t input_settings;

    void RegisterActionMap(int key, ActionMapData action_map_data) {
        this->input_settings.emplace(key, action_map_data);
    }

    void AppendAction(int key, callback_action_t callback) {
        this->input_settings[key].actions.emplace_back(callback);
    }
    void Update() {
        int i;
        InputState* states;
        for (auto action_map_data : this->input_settings) {
            states = &(action_map_data.second.input_actions[0]);
            for (i = 0; i < 4; i++) {
                if (states[i] == 0) continue;

                for (auto action : action_map_data.second.actions) {
                    action(state[i]);
                }
            }
        }
    }

    void OnEnable() {

    }

    void OnDisable() {

    }

}; 

}}

#endif // !MOD_INPUT
