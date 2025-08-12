#
# Makefile for GLFW dependency
# NOTE: At the moment this makefile only support winX
#       TODO: Implement more platforms in the future
#

export MK = $(MAKE) -C
GIT         := git
CMAKE       := cmake

# force disable of: x11, wayland, cocoa, examples and tests
CMFLAGS  := -DCMAKE_SYSTEM_NAME=Windows -DBUILD_SHARED_LIBS=ON \
            -DGLFW_BUILD_WIN32=ON -DGLFW_BUILD_COCOA=OFF -DGLFW_BUILD_WAYLAND=OFF \
			-DGLFW_BUILD_X11=OFF -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF \
			-DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
			
# Remote GIT repository
REPO        := https://github.com/glfw/glfw.git

# Misc dirs
SRCDIR  := glfw/
OUTDIR  := build/
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY   := install-deps build-deps clean-deps-install clean-deps-build

install-deps: $(SRCDIR)

# clean build dir
clean-deps-install:
	rm -rf $(SRCDIR)/

# clean install directory
clean-deps-build:
	rm -rf $(OUTDIR)/
	
build-deps: $(OUTDIR) install-deps
	$(MK) $(OUTDIR) all
	@echo "copying all GLFW lib, bin and include files"
	cp -r $(SRCDIR)/include/GLFW $(INCDIR)/
	cp $(OUTDIR)/src/glfw3.dll $(BINDIR)/glfw3.dll
	cp $(OUTDIR)/src/libglfw3dll.a $(LIBDIR)/libglfw3dll.a
	
$(SRCDIR):
	@echo "installing GLFW source from $(REPO)"
	$(GIT) clone $(REPO)
	
$(OUTDIR):
	mkdir -p $@
	$(CMAKE) -G "Unix Makefiles" -S $(SRCDIR) -B $(OUTDIR) $(CMFLAGS)

.PHONY: $(PHONY)
	