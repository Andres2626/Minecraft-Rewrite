export MK = $(MAKE) -C

export OUTDIR         = build
export ENGDIR         = core
export INCDIR         = include
export DEPDIR         = Dependencies
export DOCSDIR        = Documentation
export SCRDIR         = scripts

PHONY += help run all clean distclean core sandbox clean-core \
         clean-sandbox install-deps build-deps clean-deps-install \
		 clean-deps-build

COMPILER_TARGET	:= $(shell gcc -dumpmachine)

DEBUG ?= 0
ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG
else
CFLAGS += -DNDEBUG
endif

export CFLAGS

# detect host platform
ifeq ($(OS), Windows_NT)
ifneq (,$(findstring mingw,$(COMPILER_TARGET)))
export PLATFORM = windows
export TOOLCHAIN = mingw
else
# At the moment MC engine doesn't support cygwin or msys2 (except for msys-mingw**)
$(error platform $(COMPILER_TARGET) not supported)
endif
else
# TODO: More platform support in the future
$(error platform $(shell uname -o) not supported)
endif

all: dep_warning core sandbox

dep_warning:
	@echo Before building the MC project, you need to have the dependencies 
	@echo installed and built. To do this, use the make build-deps command 
	@echo to avoid compilation errors.
	@sleep 3

distclean: clean
	rmdir $(OUTDIR)
	
clean:
	@echo "Clearing All..."
	rm -rf $(OUTDIR)/*
	rmdir $(OUTDIR)/
	
core:
	@echo "Building engine"
	$(MK) core/ all

sandbox:
	@echo "Building sandbox"
	$(MK) sandbox/ all

clean-core:
	@echo "Clearing MC engine"
	$(MK) core/ clean
	
clean-sandbox:
	@echo "Clearing MC engine"
	$(MK) sandbox/ clean

install-deps:
	@echo "installing all dependencies"
	$(MK) $(DEPDIR)/ install-deps

build-deps:
	@echo "building all dependencies"
	$(MK) $(DEPDIR)/ build-deps

clean-deps-install:
	$(MK) $(DEPDIR)/ clean-deps-install

clean-deps-build:
	$(MK) $(DEPDIR)/ clean-deps-build

run:
	$(OUTDIR)/bin/rd-132211

include scripts/help_target.mk

.PHONY: $(PHONY)
	