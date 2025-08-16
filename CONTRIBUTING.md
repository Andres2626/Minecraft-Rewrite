# 🤝 Contributing to MC project

Thank you very much for your interest in contributing to the MC project. Any 
contribution is welcome and feel free to submit a PRs or an issue on the 
repository's main page.

> ⚠️ **NOTE**: If you decided to contribute to the code (thanks again for this) 
> this document assumes that *YOU* have read the documentation before and are 
> familiar with the code exposed in the *MC project*.
>
> Also keep in mind the [TODO](./TODO) to get an idea of the project's 'roadmap'.

## Table of contents
- [Project scope and rules](#-1️⃣--project-scope-and-rules)
- [Language](#-2️⃣--language)
- [Recommended knowledge](#-3️⃣--recommended-knowledge)
- [Reporting issues](#-4️⃣--reporting-issues)
- [Commit standards](#-5️⃣--commit-standards)
- [Submitting a PR](#-6️⃣--submitting-a-pr)

## ❗ Important note

It is referred to as a "rewritten" version like all those versions of the MC 
project that are currently rewritten in C++, and original '.jar' file exists.

A "recreated" version is when the '.jar' file is missing and there is only 
evidence in the form of videos, IRC, etc.

## 1️⃣  Project scope and rules

I encourage anyone interested to contribute as long as:

- All features that are added must be consistent with the version of 
  Minecraft that is being recreated.
- Not all versions of Minecraft THAT are online, so those that aren't 
  are considered *lost*. (*see [Ommniarchive Downloads]*(https://omniarchive.uk/downloads/)
  *for more info*)
  
  Some of these versions have evidence of existence, such as photos, 
  videos, IRC logs, ... these versions fall within the MC project but 
  are considered "recreated". Please check [Minecraft Java Versions.](https://minecraft.fandom.com/wiki/Java_Edition_version_history)
  
- At the moment the *lost* version are **EXCLUDED** from *MC project.*

## 2️⃣  Languaje

All comments, documentation, names, codes... are written in **ENGLISH** 
please follow this policy for all your PRs or issues.

## 3️⃣  Recomended knowledge

***Any contribution is welcome. but...*** I recommend that before contributing 
to the code, you should be familiar with the original code for all Java 
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
called the "rewritten/recreated version file", which indicates the version 
currently being worked on for the MC project.

*Refrain from submitting PRs that affect previous file.*

## 4️⃣  Reporting issues

For bug tracking, please write to:

> [Issues Page.](https://github.com/Andres2626/Minecraft-Rewrite/issues)
	
or

> minecraftrewrite21@gmail.com

## 5️⃣  Commit standards

Your confirmation message should look like this:

```
14-08-25 [PATCH 3] Cygwin compatibility
All makefiles are now compatible with cygwin and msys2.

For this, the files that are dependent on $(OS) and that are
included in the main makefile were created.

 ** scripts/windows-platform.mk -> window specific variables

This exports its variables via export to all makefiles marked
as $(SUBDIRS) via the $(MAKE) command.

 ** scripts/command.mk -> gcc, ld, ar, etc commands

To be compatible with Cygwin, MC now supports X11 and OpenGL3.3

* ZLIB now compiles with cmake (avoid use of native makefiles)
* Also fixed the level update reset (now resets to 0 every second)

In include/core/Host/Windows/exit.h the definition MC_INTERNAL_BREAK_POINT
was established depending on the Windows toolchain

 ** raise(SIGTRAP) for *emulated* unix enviroments (cygwin, msys2...)
 ** __debugbreak() for VS and mingw compilers

Now all comments will have the original author's signature at the
end, like this:

 ** Signed-off-by: author <email>

Signed-off-by: Andres26  <andrescamorao9@gmail.com>
```

Or:

```
14-08-2025 [PATCH 3] Fix shader bug
The fog procedure was being done incorrectly, so it was changed back
to how it was before.

 commit: 6291f9e
Signed-off-by: Andres26  <andrescamorao9@gmail.com>
```

## 6️⃣  Sending an PR

1. Fork your repository with your changes.
2. Create a new branch for your work:

```
git checkout -b feature/my-feature
```

3. Make your changes and commit them.
4. Push your branch

```
git push origin feature/my-feature
```

5. Open a PR to the main branch.
6. Wait for approval and reply to all feedback.

