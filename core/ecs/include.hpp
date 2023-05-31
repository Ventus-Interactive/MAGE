
#ifndef MOD_ECS
#define MOD_ECS

extern "C" {
    #include "lib.h"
}


#include <unordered_map>
#include <string>

#include <memory/include.hpp>

namespace MAGE {
namespace ECS {

class Scene {
public:
    template <class T>
    Memory::ArenaVar<T> CreateObject(T obj_copy) {
        int size = sizeof(T);
        l64 sp = LibFn(arena_push)(&this->arena, size, &obj_copy);
        
        if (Backend_FailArena(sp)) 
            return (void*)0;

        void* loc = &this->arena.arr[sp];
        printf("- ECS::Scene::CreateObject : (bytes):%lu, (ArenaAddr):%p, (SP): %lu\n", size, loc, sp);

        return Memory::ArenaVar<T>(loc);
    }

    void Init(int bytes);
    void Destroy();

protected:
    cArena arena;
};

//class IScenePlayer {
//public:
 //   virtual void Play(SceneManager* sceneManager) {}
//};

class ISceneLoader {//}: public IScenePlayer {
public:
    virtual void InitScene() {}
    virtual void LoadScene() {}
    virtual void Play(void* sceneManager) {}
    virtual void Destroy  () {}

protected:
    Scene scene;
};
class SceneManager {
public:
    SceneManager() {}

    template <class T>
    T* NewScene(std::string name) {
        if (this->scenes.find(name) != this->scenes.end()) return 0;

        printf("- NewScene(%s) \n", name.c_str());

        T* scene_loader = new T();// (T*)malloc(sizeof(T*));
        scene_loader->InitScene();
        this->scenes.emplace(name, scene_loader);

        return scene_loader;
    }

    void Start();
    void Play(std::string name);

    void Destroy();

private:
    std::unordered_map<std::string, ISceneLoader*> scenes;
};




class SystemPassthroughModifier {
public:
    // is registered into a system
    // the system interally call registered modifiers
    // this can be used for system manipulation without modifiying the source code

private:

};



}}

#endif // !MOD_ECS
