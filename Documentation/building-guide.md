# 🛠️ Building Guide

The following is the complete guide to compile MC and its dependencies.

## Table of contents
1. [Important notes](#-important-notes)
2. [Prerequisites for build](#-prerequisites-for-build)
3. [Prepare assets](#-prepare-assets)
4. [Install dependencies](#-install-dependencies)
5. [Building MC project](#-building-mc-project)
6. [Cleaning dependencies](#-cleaning-dependencies)
7. [CMake options](#-cmake-options)

## ⚠️ Important notes

- The *MC project* does **NOT** contain the original game assets, you 
  must obtain them yourself (see *limitation of liability* in the README 
  for more info).
- The project *as is* does **NOT** contain dependencies code. (see [Install dependencies](#-install-dependencies))
  for more info.
- The build system currently supports **Windows-like platforms** (cygwin, 
  msys2, mingw, etc). 
  
## ✅ Prerequisites for build

- cmake 3.8
- git 2.45.1
- **MC repository cloned** [Installation Guide](installation-guide.md) for more info.

## 1️⃣  Prepare assets

1. Obtain the required '.jar' file for the version being rewritten. 
2. Verify the file with **SHA-256** and compare with used '.jar'. 
   Check [used files](used-jar-files.txt) for more info.
3. Extract the JE resources in '$(PROJECT_ROOT)/assets'.
4. For older versions, you must get the icon used in the original game 
   from LWJGL library.
5. Copy your icon to '$(PROJECT_ROOT)/assets/Internal/win_icon.png'.

## 2️⃣  Install dependencies

### Unix users:

```
$ scripts/auto-install.sh
```

This downloads dependencies via git submodules in 'dependencies/' directory.

### Windows users:

Dependencies are installed automatically using CMake and FetchContent.

If FetchContent fails use:

```
$ git submodule init
$ git submodule update
```

⚠️ Important Note: If you use git submodules set MC_AUTODEP=OFF in CMake.

## 3️⃣  Building MC Project

### Unix users:

```
$ scripts/auto-build.sh
```

### Windows users:

```
$ cmake -S ./ -B ./build
$ cmake --build ./build
```

Or using 'cmake-gui' configured with your favorite IDE.

ℹ️ Note: If you installed dependencies in respective directory, you must turn 
off MC_AUTODEP in cmake.

## 🚫 Cleaning dependencies

ℹ️ Note: This **ONLY** works if MC_AUTODEP is off in cmake.

### Unix users:

```
$ scripts/clean-dependencies.sh
```

### Windows users:

Delete dependencies subdirectories manually.

## 📦 CMake options

```
MC_AUTODEP # Installs dependencies via FetchContent (default=ON).
MC_DEBUG_MODE # Enable debug mode (default=OFF)
MC_COMP_WARNS # Enable all/extra warnings and -Werror option (GCC) or /W4 in MSVC (default=OFF)
```