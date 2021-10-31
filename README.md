# csgo_internal
An old project of mine which dates back to 2-3 years ago (29-03-2019), aimed towards learning the process of game hacking in a very popular E-Sport game at that time titled Counter-Strike: Global Offensive, which is based on the Source Engine game engine that relies on legacy DirectX 9 for 3D rendering.

It is bundled with an aimbot, esp/wallhack, backtrack, glow/chams and all else that is obviously against the rules of the game.
Coupled with a weak anti-cheat which can even be blocked from loading (through SteamService.exe), its not surprising that the game is plagued with cheaters, with the only defensive measure "Trust Factor" put in place to score how much can players be trusted to play legitimately (VACnet, player analyzed replays / Overwatch, and user reports affect this score).

This was my first and only semi-complete working cheat for the game, so i expect it to have many mistakes and implemented many mediocre techniques, such as using the Detours lib from Microsoft directly instead of implementing detour hooking myself.

The cheat itself works around this lifecycle:

1. Firstly the cheat is injected to the game process (csgo.exe) with whichever dll injection method (loadlibrary, manual map, etc)
2. The cheat will initialize itself, look for signatures for internal game functions, bind internal source engine interfaces and do other stuff like looking up netvars for object struct offsets.
3. Then the cheat hooks various game functions that are called either every frame or game ticks, the cheat will run inside these.
4. After hooking these, when the player joins a game, these functions are called and the cheat runs various things, like draw the ESP / menu in D3D EndScene, run aimbot in CreateMove, and many more.
5. The cheat is unloaded upon exit, it could be made to unload itself upon request but i didnt feel it was necessary for a learning project at that time.

This project relies heavily on reverse engineering the game and some public resources from UnknownCheats when i started.

# vanilla
This incomplete project was a migration attempt to completely start over writing the code from scratch and make it follow better C++ design patterns and styles. However it is abandoned after completion of the base (only sdk, hooks, rendering and other frameworks) due to the lost of interest in making the cheat complete.