# 🛠️ Minecraft Rewrite (MC Rewrite)

![Language](https://img.shields.io/badge/language-C++-brightgreen)
![Language](https://img.shields.io/badge/language-C-brightgreen)
![Status](https://img.shields.io/badge/status-In%20Development-yellow)
![Version](https://img.shields.io/badge/version-rd132211-blue)

The MC project is an attempt to rewrite the original *Minecraft JE* code in **C++**. 
aiming for:
- **Maximum cross-platform compatibility** (like *JE*)
- **Preservation of JE original experience.**
- **Highest performance and stability**

> ⚠️ **DISCLAIMER**: This is **NOT** an *as-is* copy or decompilation of original game.
> Currently, the engine and game use newer versions of OpenGL (>3.3).
> See [Limitation of liability](#-limitation-of-liability) for more info

## 📋 Table of Contents
1. [Goals of this project](#-goals-of-this-project)
2. [Things that will not be done](#-things-that-will-not-be-done)
3. [Versions currently recreated](#-versions-currently-recreated)
4. [Contribute](#-contribute)
5. [Limitation of Liability](#-limitation-of-liability)
6. [Copyright Notice](#-copyright-notice)
7. [Directory structure](./Documentation/directory-structure.md)
7. [Install](./Documentation/installing-guide.md)
8. [Build](./Documentation/building-guide.md)
9. [Run](./Documentation/running-mc.md)
10. [Debug](./Documentation/debugging-guide.md)
11. [TODO](./TODO)
12. [Contact](./Documentation/contact-info.md)

## ✅ Goals of this project

- Rewrite all versions of the *JE* in *C++* (up to 1.0 at the moment).
- Add support and compatibility for as many platforms as possible. (like *JE*)
- Rewrite the main game code in *C++*, maintaining maximum performance and 
  stability compared to *JE*
- Write an independent game engine from scratch.
- Preserve the **functionality** of the *as-is* Java edition.
  
## 🚫 Things that will not be done

It is not intended to add/arrive at the following:

- The complete rewrite of the *BE* and its derivatives (e.g. MCPE, Xbox version, etc).
- An *as-is* copy and decompilation of the original game.
- Rewrite versions beyond R1.0 (*for now*).
- A multiplayer support bridge for *non-JE* versions.
- Adding non-original features.

## 📌 Versions currently recreated

- ***PARTIALLY*** rd-132211.
  The version is 100% written, but some important changes to the MC Engine 
  are missing:
  - Port linux.
  
## 🤝 Contribute

Thank you for your interest in contributing to the MC project. Before 
you begin contributing, please check [Contribution Guide](./CONTRIBUTING.md).

## ⚖️ Limitation of liability

This project is not affiliated with Mojang A.B. Furthermore, MC 
project is not *as-is* copy of original game code.

The project **DOES NOT** have the original game assets, so you'll 
have to find them yourself (see 'Building' for more info)

Please read the copyright notice.

## © Copyright notice

**MC Rewrite** code is © Andres26 2025. Redistribution is allowed under these terms:

- Full source code is available.

- The copyright notice MUST be intact.

- It is prohibited to distribute this for a fee.
