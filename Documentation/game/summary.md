## Controls

In this version the controls do not differ from the original version, just so you keep 
it in mind:

- 'WASD' player move.
- 'R' for to restore the player's position to an arbitrary position in the level.
- 'SPACE' for jump.
- 'ESC' for exit.
- Right click for destroy block.
- Left click for put block.

## Technical summary and features of the game

This version of the game is a complete recreation of the original Java 
version. It's open source and reimplements some features of the original code.

- Rendering process: Unlike of the original version, this use shaders and 
  and only 1 draw call per chunk, the original version that used 2 drawing calls 
  for rendering the dark and light blocks which affected the game's performance.
  
- The FOG process was moved to 'chunk.shader', this allows avoid calls to prehistoric OpenGL.

  ![Level](../resources/fog.jpg)<br>
  Fog demostration.
  
- The code does not support legacy OpenGL versions (<3.3)

- VBO indexer: An indexer is used for vertices (uses more memory in exchange for 
  fewer drawing calls to the GPU)
  
- The selector moves and rotates using the model matrix that the application 
  passes to the shader, which avoids unnecessary calls to GPU, each time the 
  cursor moves and repositions the selector on the selected block.
  
  ![Level](../resources/selector.jpg)<br>
  Block selector demostration
  
- Chunk updates (CUPS): If a block is destroyed or a block on the edge of the chunk is 
  destroyed, it has to be updated for the above reason.
  
- Selector: This works as follows first it calculates the parametric equation of the camera 
  line 'f(t) = dir t + pos' and then, using a for loop, it sets the value of 't' 
  (every 0.1f) and finally the ray collides with the block and returns the normal vector
  of the impacted face.

## Original Assets

The code *as-is* does **NOT** have the original JE assets for legal reasons. So you'll have 
to get the original assets yourself, you can check the '.jar' that was used to rewrite it here.

Once this is done copy 'terrain.png' to the assets directory and the LWJGL logo to 'assets/Internal/win_icon.png'

## Level format

This is the simplest part of all and shouldn't cause any problems between the original 
Java version and the rewritten version. Both versions are fully compatible with 'level.dat' 
and use the same compression/decompression system (using GZIP). Loading 'level.dat' is 
optimized, unlike the original version. The process is as follows:

- The code checks if a 'level.dat' file exists.

- If it exists, it is decompressed, and the resulting buffer is passed to 'level->m_Blocks'.

- If not, 'level->m_Blocks' is created from scratch, and then the buffer is compressed into 
the 'level.dat' file, which is opened in binary mode for writing.

## Chunk functions

The Chunk class only acts as a renderer for the blocks being rendered on screen. The Level 
class creates a chunk map, preventing the object from being copied and deleted using 
'm_ChunkRenderer.emplace'. This prevents unnecessary operations during chunk creation. 
Chunks have a default size of 16 blocks. When generating the array, it is verified that 
adjacent blocks are rendered at a specific frame.

## Benchmarks and tests

In my case, I managed to get an average of 300 FPS in the debug version, and in 
the release version, it averaged 400 FPS. The game also takes a while to load at 
first due to the chunk renderer and the generation of the block array in the level.

![Level](../resources/benchmark.jpg)<br>
FPS counter from my version.

## Dependencies, etc...

For those curious, the libraries used for this project are:

- GLFW 3.4
- GLAD for GLAPI=4.6 (view Include/lib/gfx/glad.h for more info :D )
- stb_image 2.30
- GLM 1.0.0
- ZLIB 1.3.1
- GLEQ for GLFW 3.0

## Run application

To run the application you must have a graphics card that supports modern versions of OpenGL (>3.3)