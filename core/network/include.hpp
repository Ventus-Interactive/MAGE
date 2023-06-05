
#ifndef MOD_NETWORK
#define MOD_NETWORK

extern "C" {
    #include "lib.h"
}

namespace MAGE {
namespace Network {

//###### Starter Framework
//- Global Stat CRUD
//  - backend for api's like leaderboards, etc..

/*- Cloud Saves

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
*/

}}

#endif // !MOD_NETWORK
