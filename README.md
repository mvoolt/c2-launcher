# c2-launcher

C2-Launcher is an open-source replacement of the original [Crysis 2](https://en.wikipedia.org/wiki/Crysis_2)
executables.

Scope of this project is to re-enable the 64-bit build found in the Crysis 2 Mod SDK to allow for playing
as a client instead of it just being in an editor.

**This is very much still WIP and may still crash but at the moment you could load into a map and use weapons.**

# TO-DO:
[ ] disable devmode by default (make sure the game starts in fullscreen and no r_displayinfo by default on new machine)
[ ] fix DX11 fps cap limit on fullscreen (one idea is to force borderless windowed for fullscreen)
[ ] check if multiplayer works or something (can we connect to servers hosted with the 32-bit binaries?)
[ ] check if we can host a dedicated server (can we host a server with this build and connect from retail 32-bit client?)
[ ] check if this actually has any improvement of fps over retail 32-bit client (i'm only making c2-launcher under the belief that 64-bit is faster)

# Thanks
ccomrade for [c1-launcher](https://github.com/ccomrade/c1-launcher) which is used as basis for this project. c1-launcher has been hard forked from commit d37f513 to easily modify for this project.
rafalh for FunHook (`vendor/funhook`) which was originally made for [Dash Faction](https://github.com/rafalh/dashfaction). Dash Faction is licensed under Mozilla Public License 2.0.
