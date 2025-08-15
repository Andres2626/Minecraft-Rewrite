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
   
5. At the moment makefile **ONLY** winX platforms (cygwin, mingw, msys2, etc)

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
