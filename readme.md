# Minecraft Rewrite (MC Rewrite)

This project is an attempt to port Minecraft Java Edition to C++ seeking 
cross-platform, maximum performance, stability and preserving the Minecraft 
experience. 

This code is based on the compilation of the original .jar versions of 
Minecraft, but this one is a rewrite from scratch starting from 
rd-132211 (view limitation of liability)

## Goals of this project

- Recreate in c++ all versions of the Minecraft Java Edition (up to 1.0 at the moment), 
  Adding the features of each version in the process.  
- Add support and compatibility for as many platforms as possible. (like Java Edition)
- Rewrite the main game code in C++, maintaining maximum performance and 
  stability (unlike the Java version)
  
## Things that will not be done

- The rewrite of the Bedrock Edition (includes support in each version).
- Rewrite beyond 1.0 (for now)

## Versions currenly recreated
- ***PARTIALLY*** rd-132211. [Release notes](./Documentation/versions/rd-132211-1/relnotes.md)
  The version is 100% written, but some important changes to the MC Engine are missing:
  - Implement Makefile build system.
  - *Final refractoring!*
  - Simple documentation.
  - Port to cygwin and linux. *(TODO list)*
  - Separate chunk buffer *(TODO list)*
  
## Contribute

Anyone interested can contribute to the project. I encourage anyone interested to contribute 
as long as:

- All features that are added must be consistent with the version of Minecraft that is being 
  recreated.
- When recreating a version of Minecraft in C++ it doesn't matter whether it's in the main 
  launcher or not, but:
	- If the version is in the launcher or marked as "archived", it is recreated as is.
	- If the version is lost and there is no evidence that it ever existed (IRC, videos, etc.) 
	  then the version cannot be recreated.
	- If the version is lost and there is evidence that it existed then, it could be 
	  recreated but not completely (especially if there is only IRC evidence)
	- If the version is recompiled, it is recreated as is.
	- For more info view [Ommniarchive](https://omniarchive.shoutwiki.com/wiki/Main_Page) and [Minecraft Java Versions](https://minecraft.fandom.com/wiki/Java_Edition_version_history) for more info.

I recommend that before contributing to the code, you  should be familiar with the original code for 
all Java versions or at least have an idea of how the code originally worked (especially early versions) and 
keep in mind the IRC logs, the original changelogs, videos, etc. of older versions (see [Ommniarchive](https://omniarchive.shoutwiki.com/wiki/Main_Page) for more info).

Below are some links that could be of great help when contributing to the project.

- [Ommniarchive](https://omniarchive.shoutwiki.com/wiki/Main_Page)
- [Minecraft Java Versions](https://minecraft.fandom.com/wiki/Java_Edition_version_history)
- [Minecraft 2009 Original logs](https://web.archive.org/web/20140601000000*/https://echelog.com/logs/browse/lwjgl/1242165600)
- [The word of notch](https://blog.omniarchive.uk/archive/)

Once the above is done and the code is ready, [send a pull request](https://github.com/Andres2626/Minecraft-Rewrite/pulls)

## Copyright Notice

All project code is (C) Andres26 2025, but the code can be redistributed if:

- Full source are available and free.

- The copyright notice MUST be intact.

- It is prohibited to distribute this for a fee.

- The above excludes the dependencies/ and assets/ directories.

If you have any question, mail me to minecraftrewrite21@gmail.com

## Limitation of liability

This project is not affiliated with Mojang A.B. Furthermore, the project code is not a 
copy of the code contained in the original code's .jar files.

## Prerequesites for build

- Visual Studio At least the version I use to compile the code is 2019

To build the project, you don't need any specific dependencies, as the project 
already includes the dependencies, so you don't need to install and configure 
them yourself.

At the moment, this version is only available for WIN32 (TODO: implement platform) because 
GLFW's dependencies are included only for Windows.

For those curious, the libraries used for this project are:

- GLFW 3.4
- GLAD for GLAPI=4.6 (view Engine/External/glad.h for more info :D )
- stb_image 2.30
- GLM 1.0.0
- GZIP 0.2.0
- ZLIB 1.3.1
- GLEQ for GLFW 3.0

## Run application

To run the application you must have a graphics card that supports modern versions of OpenGL (>3.3)

## Contact 

Any questions, reports or clarifications, mail me at minecraftrewrite21@gmai.com or in the [discussion page.](https://github.com/Andres2626/Minecraft-Rewrite/discussions)
