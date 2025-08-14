# Minecraft Rewrite (MC Rewrite)

The MC project is an attempt to rewrite the original Minecraft code in C++. 
MC aims to achieve the widest cross-platform compatibility  (as in the Java 
version), preserve the *original* Minecraft experience, and, above all, 
achieve the highest performance and stability for the game.

**DISCLAIMER**: This is not intended to be an *as-is* copy of the original 
game code; it is a complete and total rewrite in C++ (***see limitation of*** 
***liability***). Currently, the engine and game use newer versions of OpenGL 
(>3.3).

## Goals of this project

- Recreate in c++ all versions of the Minecraft Java Edition (up to 1.0 at
  the moment), 
  Adding the features of each version in the process.  
- Add support and compatibility for as many platforms as possible. (like 
  Java Edition)
- Rewrite the main game code in C++, maintaining maximum performance and 
  stability (unlike the Java version)
- Creating a game engine from scratch.
- Preserve the **functionality** of the *as-is* Java edition
  
## Things that will not be done

It is not intended to add/arrive at the following:

- The complete rewrite of the Bedrock edition and its derivatives 
  (e.g. MCPE, Xbox version...)
- An *as-is* copy and decompilation of the original game.
- Rewrite versions beyond R1.0 (*for now*)
- A multiplayer support bridge for non-Java Edition versions
- Adding features that are not in the original code.

## Versions currenly recreated

- ***PARTIALLY*** rd-132211.
  The version is 100% written, but some important changes to the MC Engine 
  are missing:
  - Implement Makefile build system.
  - *Final refractoring!*
  - Simple documentation.
  - Port to cygwin and linux.
  - Separate chunk buffer
  
## Contribute

Anyone interested can contribute to the project. I encourage anyone 
interested to contribute as long as:

- All features that are added must be consistent with the version of 
  Minecraft that is being recreated.
- When recreating a version of Minecraft in C++ it doesn't matter 
  whether it's in the main launcher or not. For the moment, the versions 
  not included in this project are considered *lost* (*see [Ommniarchive Downloads](https://omniarchive.uk/downloads/)*
  *for more info*)
  
  *The versions that will not be recreated for the moment are those that*
  *do not have the original .jar.*
  
*Any contribution are welcome. but...* I recommend that before contributing 
to the code, you should be familiarwith the original code for all Java 
versions or at least have an idea of how the code originally worked 
(especially early versions) and keep in mind the IRC logs, the original 
changelogs, videos, etc. of older versions (*see [Ommniarchive](https://omniarchive.shoutwiki.com/wiki/Main_Page) for more info*).

Below are some links that could be of great help when contributing to 
the project.

- [Ommniarchive](https://omniarchive.shoutwiki.com/wiki/Main_Page)
- [Minecraft Java Versions](https://minecraft.fandom.com/wiki/Java_Edition_version_history)
- [Minecraft 2009 Original logs](https://web.archive.org/web/20140601000000*/https://echelog.com/logs/browse/lwjgl/1242165600)
- [The word of notch](https://blog.omniarchive.uk/archive/)

Also, keep in mind that each tag in this repository contains a file 
called the "recreated version file", which indicates the version 
currently being worked on for the MC project.

Refrain from submitting pull requests that affect previous file.

Once the above is done and the code is ready, [send a pull request](https://github.com/Andres2626/Minecraft-Rewrite/pulls)

## Limitation of liability

This project is not affiliated with Mojang A.B. Furthermore, MC 
project is not *as-is* copy of original game code.

The project **DOES NOT** have the original game assets, so you'll 
have to find them yourself (see 'Building' for more info)

Please read the copyright notice.

## Copyright Notice

MC project code is (C) Andres26 2025, but the code can be 
redistributed if:

- Full source are available and free.

- The copyright notice MUST be intact.

- It is prohibited to distribute this for a fee.

## Building

Read [Building Guide](./Documentation/building-guide.md) for more info.

## Run application

Read [Running MC](./Documentation/running-mc.md) for more info.

## TODO List

view [TODO](./TODO)

## Contact 

Any questions, reports or clarifications, mail me at 
minecraftrewrite21@gmai.com or in the [discussion page.](https://github.com/Andres2626/Minecraft-Rewrite/discussions)
