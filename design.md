Cotsb Server Design
===================

- Perhaps we have no callbacks and leave the specific instances where we want a callback to special subsystems.
  Such as map loading or any asset loading. The same also applies for if a command fails, that is up to each command.
Use Cases
---------

Client to server:
- Messages : Broadcasted, no callback required.
- Private messages : Single target, callback?
- Request for map: Returns to client, callback.
- Movement: No callback.
- Actions: No callback.

Server to client:
- Sync operations: At some point we need to synchronise the movements of the players with each client.


