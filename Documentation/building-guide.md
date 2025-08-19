# 🛠️ Building Guide

The following is the complete guide to compile MC and its dependencies.

## Table of contents
1. [Important notes](#-important-notes)
2. [Developer Build system note](#-developer-build-system-note)
3. [Prerequisites for build](#-prerequisites-for-build)
4. [Prepare assets](#-prepare-assets)
5. [Install and build dependencies](#-install-and-build-dependencies)
6. [Building MC project](#-building-mc-project)
7. [Makefile targets](#-makefile-targets)

## ⚠️ Important notes

- The *MC project* does **NOT** contain the original game assets, you 
  must obtain them yourself (see *limitation of liability* in the README 
  for more info).
- The project *as is* does **NOT** contain dependencies code. your need 
  install and build dependencies using 'make install-deps' or 'make build-deps'.
- The build system currenly supports **Windows-like platforms** (cygwin, 
  msys2, mingw, etc). 
  
## 📢 Developer build system note

This project uses a **custom makefile** instead of *CMAKE* or other build 
systems for technical reasons.

> - Ability to automatically manage dependencies (auto-install and auto-build)
>   interface.
> - Full control of all aspects of the build.
> - Simplified setup and building, without requiring additional *CMAKE* scripts.

## ✅ Prerequisites for build

- GNU GCC 13.4.0
- GNU Coreutils 9.0
- GNU binutils 2.45
- cmake 3.28.3
- git 2.45.1
- make 4.4.1
- **You must have installed the MC project from the oficial repository.** see 
  [Installation Guide](installation-guide.md) for more info.

ℹ️ Notes: 
- *CMAKE* is required to build third party libraries (GLFW and ZLIB).
- *GIT* is required for dependencies auto-install interface.

## 1️⃣  Prepare assets

1. Obtain the required '.jar' file for the version being rewritten. 
2. Verify the file with **SHA-256** and compare with used '.jar'. 
   check [used files](used-jar-files.txt) for more info.
3. Extract the JE resources in $(CD)/assets.
4. For older versions, you must get the icon used in the original game 
   from the LWJGL library.
5. Copy your icon to $(CD)/assets/Internal/win_icon.png

## 2️⃣  Install and build dependencies

To **ONLY** install dependencies run:

```
$ make install-deps
```

To install and build dependencies run:

```
$ make build-deps
```

## 3️⃣  Building MC Project

To build all MC project (core and sandbox) run:

```
$ make all
```

ℹ️ Notes: 

- Running 'make all' does not install or build dependencies. Run before 'make build deps'
  for avoid compilation errors. You should see a message like this:
  
> Before building MC project, you need to have the dependencies
> installed and built. To do this, use 'make build-deps' command
> to avoid compilation errors.
>
> see 'make help' for more info

## 📦 Makefile targets

Run 'make help' for view all available rules. 

```
Cleaning Targets:
clean              - Remove contents of build/ directory
distclean          - Remove build/ directory
clean-core         - Remove MC core build files
clean-sandbox      - Remove game build files
clean-deps-install - Remove all install files in Dependencies/ directory
clean-deps-build   - Remove all built files in Dependencies/ directory

Building Targets:
all                - Build all MC project files. this doesn't include Dependencies/ directory
                     Before you build MC project you MUST install dependencies yourself or using
                     make build-deps
core               - Build MC engine
sandbox            - Build Game
build-deps         - Build and install all in the Dependencies/ directory

Install Targets:
install-deps       - Install Dependencies/ from local repositories (not build)

Misc Targets:
help               - View help message
run                - Run game
make DEBUG=1       - Enable debug mode
```
