# Game of Life

## Build (Tested on Ubuntu 20.04)
### Dependencies
- [GLFW](https://github.com/glfw/glfw)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [glad](https://github.com/Dav1dde/glad)
- [glm](https://github.com/g-truc/glm)

Might need to be installed manually:
> For GLFW
>
> apt install xorg-dev

### To build the application:
Clone repository with dependencies as submodules
```
git clone --recurse-submodules https://github.com/laurikos/game_of_life.git CLONE-DIR
cd <CLONE-DIR>

mkdir build
cd build
cmake ../
make -jN
```

## Run the application:
> ,/GameOfLife

## Application usage:
When application starts there should be ImGui window to provide the information
to setup the Game of Life and providing the details about how to run game.

- Decide the game area width and height.
(No more than 10k cells can be created - arbitary limit set be me.)
- Select the initial cells to activate when the game starts
- Click the apply button on the top and the Game should start.

### Keybindings:
```
Arrow keys to move camera
Mouse wheel to zoom in/out
'x' proceed a step in game in manual mode
'space' toggle mode between manual/automatic
'r' If manual mode is toggled on -> initializes game area again with random cells
'esc' close the program (Also the 'x' button on window does the same)
```
