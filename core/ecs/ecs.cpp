
#include <_common/sdebug.h>
#include "include.hpp"

void mod_ecs_init() {
    PrintD("Mod::ecs_init")
}

void Test_mod_ecs_init() {
    PrintD("Test::ecs_init")
}


namespace MAGE {
namespace ECS {

void Scene::Init(int bytes) {
    this->arena = LibFn(new_arena)(bytes);
}
void Scene::Destroy() {
    LibFn(arena_dispose)(&this->arena);
}

void SceneManager::Start() {
    if (this->scenes.find("main_menu") != this->scenes.end())
        this->scenes["main_menu"]->Play(this);
    else
        printf("-  Fatal:: Failed to Start Scene Player, No Entry 'main_menu'\n");
}

void SceneManager::Play(std::string name) {
    if (this->scenes.find(name) != this->scenes.end())
        this->scenes[name]->Play(this);
    else
        printf("- Fatal:: SceneManager could not Play Scene<%s>(Unavailable)\n", name.c_str());
}

void SceneManager::Destroy() {
    for (auto& scene : this->scenes) {
        scene.second->Destroy();
        free(scene.second);
    }
}

}
}

