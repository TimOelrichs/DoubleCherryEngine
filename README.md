# DoubleCherryEngine
An attempt to create an eventdriven framework for multiinstance libretro cores

Work in progress - Feedback and contributions are welcome!

I created this project to experiment with an event-driven architecture for libretro cores, allowing multiple instances of cores to run simultaneously. 
The goal is to provide a framework that simplifies the management of core instances and their interactions.

- This project is not intended to be a fully functional emulator or a complete libretro core.
- It is a framework to facilitate the development of multi-instance libretro cores for
	- emulatores to use multiple instances of the same core  (Linkcable GB,GBA,GG,LINX,PSX..)
	- in the future maybe even instances of different cores (GameCube/GBA, Wii/NDS, NDS/Pokewalker... )

# Features
- Multi-instance support
- Event-driven architecture
	- onLoadGame
    - onReset
    - onBeforeRun
    - onBeforeVideoRender
    - onBeforeAudioRender
    - onOptionsUpdate
    - onSerialize
    - onUnserialize
- Core management
- Core communication
- Core configuration
- Core state management
- Core input handling
- ...
