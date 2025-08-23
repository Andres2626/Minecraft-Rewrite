# ▶️ Running MC

Once you have installed and built dependencies and MC project, you can run the
game launching in $(CMAKE_BUILD_DIR)/game/rd-132211.

## ⚠️ Important notes

- There's no need to copy the libraries to the executable directory. cmake does that for you.
- If you run the executable directly, you have to copy the assets into the directory where 
  the game is running.

### Windows users

- In VS, CMake sets the working directory to repository folder, so that the game finds the 
  assets correctly.
  
### Unix users

You can run:

```
$ $(CMAKE_BUILD_DIR)/game/rd-132211
```

This avoids copying the assets directly to the directory where the executable is located.