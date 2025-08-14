# Building process

Once you have obtained the code from the main repository, you can begin building 
the project.

NOTE: Before building the project code, it is necessary to keep the following in 
mind:

1. The MC project does not contain the original game assets, so you will have to 
   find them yourself (***see limitation of liability***).

2. The project *as is* does not contain the dependency codes. You do not need to 
   install the dependencies yourself, as the *makefile*  automatically downloads 
   all the dependencies and builds them using a special rule.

3. Check the .jar file from which the assets were extracted.
   
   For more caution see the sha256 of the original .jar files here
   
   [used files](./used-jar-files.txt)
   
4. Once you have the original game assets, copy them to the *$CD/assets* 
   directory.
   
5. At the moment makefile **ONLY** supports MINGW toolchain, because GLFW build
   is forced for build for winX platforms
   
   NOTE: This *not* work in cygwin or msys2 (except msys2-mingw), therefore no 
   X11, wayland or similar (TODO: Implement this)

Use:

```

$ make install-deps # for installing dependencies

$ make build-deps # for building and installing dependencies

```

3. Note that by running *make all* you are not running install-deps and build-deps. 
So before running *all* you must run *build-deps*.

To build the MC project:

```
$ make all
```

Additionally, the makefile has other special rules (use each one carefully to avoid
compilation errors)

*The following is documented in scripts/help_target.mk*

Cleaning Targets:<br />
clean              - Remove contents of build/ directory<br />
distclean          - Remove build/ directory<br />
clean-core         - Remove MC core build files<br />
clean-sandbox      - Remove game build files<br />
clean-deps-install - Remove all INSTALLED files in Dependencies/ directory<br />
clean-deps-build   - Remove all BUILDED files in Dependencies/ directory<br />
<br />
Building Targets:
all                - Build all MC project files. this doesn't include Dependencies/ directory<br />
                     Before you build MC project you MUST install dependencies yourself or using<br />
                     make build-deps<br />
core               - Build MC engine<br />
sandbox            - Build Game<br />
build-deps         - Build and install all in the Dependencies/ directory<br />
<br />
Install Targets:<br />
install-deps       - Install Dependencies/ from local repositories (not build)<br />
<br />
Misc Targets:<br />
help               - View help message<br />
run                - Run game<br />
make DEBUG=1       - Enable debug mode<br />
