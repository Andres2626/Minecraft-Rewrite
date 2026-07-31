# ▶️ Running MC

Once you have installed and built dependencies and MC project, you can run the
game launching in '$(CMAKE_BUILD_DIR)/game/rd-132328.'

## ⚠️ Important notes

- There's no need to copy the libraries to the executable directory. cmake does that for you.
- If you run the executable directly, you have to copy the assets into the directory where 
  the game is running.
- To run the application you must have a graphics card that supports modern versions of OpenGL (>3.3)

### Windows users

- In VS, CMake sets the working directory to repository folder, so that the game finds the 
  assets correctly.
  
### Unix users

You can run:

```
$ $(CMAKE_BUILD_DIR)/game/rd-132328
```

This avoids copying the assets directly to the directory where the executable is located.

## Web users

⚠️ *IMPORTANT NOTE*: Before doing this, it is necessary to build the game for the Emscripten platform.

To launch the game on the web, you need to set up a server in the folder where the game's 
'.html' file was generated. This is because some browsers do not allow the '.wasm' file 
to be executed locally. You can run:

```
$ cd $(CMAKE_BUILD_DIR)/game && python -m http.server 8000
```

## Controls

- '1' Select stone.
- '2' Select dirt.
- '3' Select stone brick.
- '4' Select wood.
- '6' Select bush.
- 'WASD' Player move.
- 'SPACE' Jump.
- 'R' Restore the player's position to an arbitrary position in the level.
- 'ESC' Exit.
- 'ENTER' Save level.
- 'G' Spawn human.
- 'Left Click' Push block.
- 'Right Click' Break block.