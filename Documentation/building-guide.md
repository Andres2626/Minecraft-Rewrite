# 🛠️ Building Guide

The following is the complete guide to compile MC and its dependencies.

## Table of contents
1. [Important notes](#-important-notes)
2. [Developer Build system note](#-developer-build-system-note)
2. [Prerequisites for build](#-prerequisites-for-build)
2. [Prepare assets](#-prepare-assets)
2. [Install and build dependencies](#-install-and-build-dependencies)
2. [Building MC project](#-building-mc-project)
2. [Makefile targets](#-makefile-targets)

## ⚠️ Important notes

- The *MC project* does **NOT** contain the original game assets - you 
  must obtained them yourself (see *limitation of liability* in the 
  README for more info).
- The project *as is* does **NOT** contain dependencies code. your need 
  install dependencies using makefile special rule.
- The build system currenly supports **Windows-like platforms** (cygwin, 
  msys2, mingw, etc). 
  
## 📢 Developer build system note

This project uses a **custom makefile** instead of *CMAKE* or other build 
systems for technnical and several reasons.

> - One of the reasons why the MC project prefers makefiles instead of 
>   using *CMAKE* is the ability to automatically manage dependencies, 
>	adding an auto-install and auto-build interface, simplifying things 
>	a lot when building with make as 
>   opposed to *CMAKE*.
> - Full control of all aspects of the BUILD.
> - Simplified setup, without requiring additional *CMAKE* scripts.
> - Simplicity of building the **MC project**.

## ✅ Prerequisites for build

- GNU GCC 13.4.0
- GNU Coreutils 9.0
- GNU binutils 2.45
- cmake 3.28.3
- git 2.45.1
- make 4.4.1
- "Emulated" UNIX environment in winX (cygwin, msys2) or mingw
- **You must been INSTALLED MC project from oficial repository.** see 
  [Installation Guide](installation-guide.md) for more info. *MC Project* does **NOT** support non winX platforms.  

Although we would like to avoid using cmake as much as possible in the MC 
project, it is necessary because compiling *GLFW* and *ZLIB* require *CMAKE* to 
configure the build environment.

*GIT* in this case is nesesary because auto-install downloads dependencies 
from a remote *GIT* repository.

## 1️⃣  Prepare assets

1. Obtain the original Minecraft Java edition '.jar' for the version being
   rewrited.
2. Verify the file with **SHA-256** and compare with used '.jar'. 
   check [used files](used-jar-files.txt) for more info.
3. Extract the JE resources in $(CD)/assets.

## 2️⃣  Install and build dependencies

If you **ONLY** install dependencies run:

```
$ make install-deps
```

On the other hand, if your intention is install and build all the dependencies, 
run:

```
$ make build-deps
```

ℹ️ Note: Running *make all* does **NOT** install and build dependencies, you must
run *make build-deps* before *make all*. The above is necessary to avoid compilation 
errors.

## 3️⃣  Building MC Project

```
$ make all
```

This build all *MC project* without dependencies.

ℹ️ Note: In case you have forgotten that you need to install the dependencies before 
starting *all* the program always reminds you with a message like this:

> Before building the MC project, you need to have the dependencies
> installed and built. To do this, use the make build-deps command
> to avoid compilation errors.
>
> see 'make help' for more info

*After this the makefile stops for 3 seconds*

## 📦 Makefile targets

Additionally, the makefile has other special rules (use each one carefully to avoid
compilation errors)

If you run the *help* rule you should see a message like this:

```
Cleaning Targets:
clean              - Remove contents of build/ directory
distclean          - Remove build/ directory
clean-core         - Remove MC core build files
clean-sandbox      - Remove game build files
clean-deps-install - Remove all INSTALLED files in Dependencies/ directory
clean-deps-build   - Remove all BUILDED files in Dependencies/ directory

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
