## Overview

This project is a [Super Pac-Man](https://en.wikipedia.org/wiki/Super_Pac-Man) clone using the [Infinite Motion Engine](https://github.com/KwenaMashamaite/IME), 
a 2D game engine that I developed. This game uses the same core logic as my [clone](https://github.com/KwenaMashamaite/PacMan) 
of the original Pac-Man game.

## How to play

1. [Download](https://github.com/KwenaMashamaite/SuperPacMan/releases/tag/v1.0.0)
   the latest game version
2. Extract the zip archive
3. Run _SuperPacMan.exe_
4. Enjoy :video_game:

## Screenshots

1. Startup Scene

![](docs/screenshots/startup_scene.png)

2. Main Menu Scene

![](docs/screenshots/main-menu_scene.png)

3. Gameplay Scene

![](docs/screenshots/gameplay.gif)

4. Pause Menu Scene

![](docs/screenshots/game_pause_scene.png)

4. Game Over Scene

![](docs/screenshots/game_over_scene.png)

## Build

###### *Read this section if you want to build the game yourself, otherwise you may skip it.*

This project is a CMake project, therefore you must download and install [CMake](https://cmake.org/). 
You also need to download [Infinite Motion Engine v2.4.0](https://github.com/KwenaMashamaite/IME/releases/tag/v2.4.0). 
Then:

1. Clone the repository
```git
$ git clone https://github.com/KwenaMashamaite/SuperPacMan
```   
2. Create a folder in the root of the directory created in step 1 and name it `extlibs`
3. Extract **IME** to the `extlibs` folder created in step 2. It should look like:

![](docs/screenshots/extlibs_directory_structure.png)
   
At this point your ready to build the project. You may use an IDE that supports building with *CMake*.
or the *CLI* using the the following commands:

```shell
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will can be found in the `bin` directory

## Platform

This game only runs on the Windows platform. It was tested on Windows 10.
