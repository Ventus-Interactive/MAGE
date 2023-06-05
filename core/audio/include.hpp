
#ifndef MOD_AUDIO
#define MOD_AUDIO

extern "C" {
    #include "lib.h"
}


#include <unordered_map>
#include <string>
#include <vector>


#include <_common/raylib.h>
#include <lua.hpp>

#include <_common/interfaces.hpp>


namespace MAGE {
namespace Audio {

/// load module-wrappers to Lua
void LoadModule(lua_State* L);


class SoundResource : public IResource {
public:
    SoundResource(Sound sound) : sound(sound) {}
    Sound* Get() { return &(this->sound); }
    void Destroy() {
        PrintD("Destroying Sound")
        UnloadSound(this->sound);
    }

private:
  Sound sound;

};

class MusicResource : public IResource {
public:
    MusicResource(Music music) : music(music) {}
    Music* Get() { return &(this->music); }
    void Destroy() {
        PrintD("Destroying Music")
        UnloadMusicStream(this->music);
    }

private:
  Music music;

};



class Settings {
public:
  float volume;
  float pitch;
  float pan;

};

class SoundPlayer {
public:
  //- register sound method
  void RegisterSound(std::string sound_name, SoundResource* sound) {
      this->sound_map.emplace(sound_name, sound);
  }

  //- play sound method
  void Play(std::string sound_name) {
      PlaySound(*(this->sound_map[sound_name]->Get()));
  }
  
  //- stop sound method
  void Stop(std::string sound_name) {
      StopSound(*(this->sound_map[sound_name]->Get()));
  }

private:
  //- map for sound resources 
  std::unordered_map<std::string, SoundResource*> sound_map;

  //- mixer settings
  Settings sound_settings; 

};


 


class MusicPlayer {
public:

class Layer : public ITogglable {
public:

    void ToggleState() { this->active = !this->active; }

    //  - method for registering music
    void RegisterMusic(std::string music_name) {
      this->music_list.emplace_back(music_name);
    }

    //  - starts playing each music
    void Play(MusicPlayer* player) {
      for (auto m : this->music_list) {
          PlayMusicStream(*(player->GetMusic(m)));
      }
    }

    //  - stops playing each music
    void Stop(MusicPlayer* player) {
      for (auto m : this->music_list) {
          StopMusicStream(*(player->GetMusic(m)));
      }
    }

    //- mixing settings
    Settings layer_settings;

    //- has list of keys to musics
    std::vector<std::string> music_list;

    bool active = true;

};

class Track : public ITogglable {
public:

    void ToggleState() { this->active = !this->active; }

    //- register Layer method
    void RegisterLayer(std::string layer_name, Layer layer) {
      this->layer_map.emplace(layer_name, layer);
    }
    
    void RegisterLayerMusic(std::string layer_name, std::string music_name) {
      this->layer_map[layer_name].RegisterMusic(music_name);
    }
    
    void ToggleLayer(std::string layer_name) {
      this->layer_map[layer_name].ToggleState();
    }

    //- has global settings like Volume, Pitch, Pan
    Settings track_settings;

    //- has a map of Layer's
    std::unordered_map<std::string, Layer> layer_map;

    bool active = true;

};


  //- register audio_data method
  void RegisterMusic(std::string music_name, MusicResource* music) {
      this->music_map.emplace(music_name, music);
  }  

  //  - calls play method on each enabled layer in track_name's Layer's
  void PlayTrack(std::string track_name) {
    for (auto layer : this->track_map[track_name].layer_map) {
        layer.second.Play(this);
    }
  }

  //  - calls stop method on each enabled layer in track_name's Layer's
  void StopTrack(std::string track_name) {
    for (auto layer : this->track_map[track_name].layer_map) {
        layer.second.Stop(this);
    }
  }
  
  void ToggleTrack(std::string track_name) {
      this->track_map[track_name].ToggleState();
  }
  
  void ToggleTrackLayer(std::string track_name, std::string layer_name) {
    this->track_map[track_name].layer_map[layer_name].ToggleState();
  }
  
  void RegisterTrack(std::string track_name, Track track) {
      this->track_map.emplace(track_name, track);
  }

  void RegisterTrackLayer(std::string track_name, std::string layer_name, Layer layer) {
      this->track_map[track_name].RegisterLayer(layer_name, layer);    
  }

  void RegisterTrackLayerMusic(
    std::string track_name, 
    std::string layer_name, 
    std::string music_name
  ) {
      this->track_map[track_name].RegisterLayerMusic(layer_name, music_name); 
  }

  Music* GetMusic(std::string music_name) {
      return this->music_map[music_name]->Get();
  }

private:
  //- maps for music resources
  std::unordered_map<std::string, MusicResource*> music_map;

  std::unordered_map<std::string, Track> track_map;

};

}}

#endif // !MOD_AUDIO
