
# M.A.G.E.
## Modular Accessible Game Engine



# Made for Linux/Ubuntu & RaspberryPi-4
- Developed using Linux, for Linux users.
  - (currently only Linux support. may not function as intended when using Windows/WSL or MacOS)
- Low-cost systems allow distribution on RaspberryPi 3 & 4, possibly other similar devices as well

## Write 2D/3D Games and Simulations with Native C++ and/or Lua
- Powered by Raylib
  - raylib has no external dependencies, this project only depends on raylib.
  - minimal total wrapping over any raylib methods

- Lightweight, Fast, and Safe
  - Written in 'Golden C++' (minimal usage of Standard & Template Libraries, and newer language features)
  - Carefully designed to minimize bloat
  - Core code aims to be considered 'Pure C'
  - Most allocated memory is managed through MemoryArena's (via ArenaManager's)

- Modular Design
  - Modular design is good for contributors :)
  - The Codebase is split into modules and sub-modules for organization

- Lua Scripting 
  - Simple Module APIs
    - enable functionality from a module for a Lua environment
  - Faster interation
    - Lua scripts can be rewritten to change the behavior of an app without recompiling it 



#### Common (not a module)
- types
    - i32, u32
    - f32, f64
    - l64
    - byte_arr_t
    - size_pair_t

- defines
    - app_main
      - for quickly starting a project
    - library helpers
      - LibFn
        - used to call C methods

- logging
    - logging can be disabled
    - log to console
    - log to file (append)


#### Assets (module)
###### Lua Wrappers for raylib 
- Blender Support
- wrapper loading methods
  - meshes
  - models
  - textures
  - audio
  - prefabs
  - scenes


#### Audio (module)
###### Customizable Audio Players

- SoundPlayer class
  - maps for sound resources 

  - has mixer settings
    - AudioSettings 

  - register sound method
      - load <asset_name, sound>

  - play sound method
    - play sound_name


- MusicPlayer class
  - maps for music resources

  - register audio_data method
    - load <asset_name, music> 

  - play_track method
    - calls play method on each enabled layer in track_name's Layer's

  - stop_track method
    - calls stop method on each enabled layer in track_name's Layer's

  - toggle_track method
    - toggle track_name

  - toggle_track_layer method
    - toggle track_name's layer_name

  - register track method
    - emplace <track_name, track_data>

  - register track_layer method
    - call track_name's register method

  - register track_layer_audio method
    - call track_name's layer_name's register music method

  - Track
    - can be toggled
    - has global settings like Volume, Pitch, Pan
      - AudioSettings
    - has a map of Layer's
    - register Layer method
    - LoadLayer(mix_layer, music)
    - ToggleLayer(mix_layer)

  - Layer
    - can be toggled
    - has all settings
      - AudioSettings

    - has list of keys to musics
      - method for registering either  music

    - play method
      - starts playing each music

    - stop method
      - stops playing each music


#### ECS (module)
###### Entities, Components, Systems, and Resources
 - IEntity ( ITogglable, IUpdatable )
    - HasComponent(name)
    - GetComponent(name)
    - GetArenaSpan()
      - sp <--> sp+size
- World ( IDestroyable, ITogglable, IUpdatable )
  - ArenaManager for Component's 
    - GetComponentSP()

  - unordered_map's for <name, IEntity>, <name, IResource>
    - AddEntity(name, data)
    - GetEntity(name)
    - RemoveEntity(name)
    - AddResource(name, data)
    - GetResource(name)
  
  - Destroy
    - clean components, resources
  
  - ApplySystem(sys)
    - run a custom system with entities*, resources* param

#### Events (module)
###### Queue/Listener(s) Event System
  - event queue
  - register listeners to be invoked when <event> happens
    - pipe the data through the listener
  - trigger events from methods (with args)

#### GUI (module)
###### UI Widget Toolkit
  - grid
  - panel
  - image
  - button
  - slider
  - toggle
  - text
  - input text


#### Input (module)
###### Keyboard/Mouse, Gamepads, and Touch Devices
- struct input_map_data {input_methods_mask, callback_list}
- typedef <key, input_map_data> input_settings
- manager class ( ITogglable, IUpdatable )
  - contains an input_settings
    - can append input_map_data's
  - polls input maps
    - invoke map's activated callback


#### Memory (module)
###### Safe Resource Management
- cArena ( C )
  - contains a pointer for N bytes
  - uses a stack pointer for variable allocation
  - tracks its capacity
  - can be cleared for reuse
- ArenaVar<T> ( C++ )
  - wrapper for data located inside a cArena
  - Get()     gives you a data pointer
  - Unwrap()  gives you a data copy
- arena backend methods
  - new
  - dispose
  - print
  - push
  - clear
- ArenaManager ( C++ ) ( IDestroyable )
  - container CRUD
    - object clear method
      - store a memory region from sp_begin to sp_end  ( loc, loc+size )
    - dynamic create method
      - check memory regions
        - if the new object is too big, do a normal arena push
        - if the object fits, resize the region to start after this new object
  - wraps a cArena
  - wrapper OOP methods
    - new
    - dispose
    - print
    - push
    - clear

#### Network (module)
###### Starter Framework
- Global Stat CRUD
  - backend for api's like leaderboards, etc..

- Cloud Saves

- OTA(over-the-air) Game Reloading

- Rooms
  - clients join rooms
    - members can communicate in and interact with the Room
  - rooms have rules which are initialized by the host
    - room rules are propagated to clients
    - the rules are the shared agreement for the players to stay in sync

- Worlds
  - worlds are actual running instances of a scene/timeline

- Users can host rooms and worlds

- Synchronized Variables
  - owned by the server
  - data changes will be propagated to clients
  - can only be set by auth
    - clients can request to set the data


#### Physics (module)
###### Rigidbodies, Forces, Raycasting, and AABB Collision Systems
- Global Forces (wind, water, gravity, drag, etc..)

- Collision/Trigger Callbacks
  - Entities can assign a callback or callback group
    - a callback is function signature
    - a callback group is a group of entities that share a single callback

- Box Sweep, Raycast

- Sphere Collision

- 2D/3D Particle physics


- Default Physics/Collision Systems
  - solves collision and physics forces
  - Filters through a World to find entities with physical characteristics
  - AABB Spacial Partitioning
  - Attempts to invoke callbacks when entities collide
  - Physics is calculated on Rigidbody/Collider Entities


#### Rendering (module)
###### Dynamic Pipeline Renderer, Models, Animations, Billboards, Sprites, Particles
- Screen Effects

- Procedural Meshes

- Particle Rendering systems

- Billboard Quads & Meshes

- Animation Systems

- Imposter rendering
  - renders a single object from multiple angles and projects it onto several quads
  - one shared object for many instances
    - the piece of geometry can be loaded a single time and be reused

- cSpriteData            
- cMeshData
- cModelData
- cAnimatorData
- SpriteRenderer ( IDestroyable, ITogglable, IUpdatable )
- MeshRenderer   ( IDestroyable, ITogglable, IUpdatable )
- ModelRenderer  ( IDestroyable, ITogglable, IUpdatable )
- Animator       ( IDestroyable, ITogglable, IUpdatable )

- Shader & Material Tables
- Assignable Materials/Shaders

- Default System:
    - Objects are filtered out of a World
        - the objects are rendered correctly based on attributes




#### Scripting (module)
###### Make your games Moddable
- Scripting API
  - should mirror native scripting as much as possible
  - IScriptBehavior
    - virtual methods:
      - init      ( IDestroyable )
      - on-enable  ( ITogglable )
      - on-update (  IUpdatable  )
      - on-disable ( ITogglable )
      - destroy   ( IDestroyable )
  - class to wrap LuaState ( IDestroyable )
    - init, get, close
  - class for each module that exposes to LuaEnv
    - loads module's script into LuaState
    - will string_call or method_invoke load/init and clean_up/close


#### Threading (module)
###### Task management for parallel system execution
- may split homogenous system functions invokes into batch calls


#### Window (module)
###### Screen/Window Helpers & Lua Wrappers
- Utilities for screen related calculations
- Lua wrappers for Screen/Window related methods


