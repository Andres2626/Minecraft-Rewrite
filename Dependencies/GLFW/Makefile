#
# Makefile for GLFW dependency
# NOTE: At the moment this makefile only support winX
#       TODO: Implement more platforms in the future
#

# force disable of wayland
CMFLAGS  := -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_WAYLAND=OFF -DGLFW_BUILD_EXAMPLES=OFF \
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
	
build-deps: install-deps $(OUTDIR) 
	$(MK) $(OUTDIR) all
	@echo "copying all GLFW include files"
	cp -r $(SRCDIR)/include/GLFW $(INCDIR)/
	
	@echo "copying all GLFW .$(LIBEXT) files"
	@for f in $(shell find $(OUTDIR)/src -name '*.$(LIBEXT)'); do \
		cp $$f $(LIBDIR)/; \
	done
	
	@echo "copying all GLFW .$(DYNEXT) files"
	@for f in $(shell find $(OUTDIR)/src -name '*.$(DYNEXT)'); do \
		cp $$f $(BINDIR)/; \
	done
	
$(SRCDIR):
	@echo "installing GLFW 3.4 source from $(REPO)"
	$(GIT) clone --depth 1 --branch 3.4 $(REPO)
	
$(OUTDIR):
	mkdir -p $@
	$(CMAKE) -G "Unix Makefiles" -S $(SRCDIR) -B $(OUTDIR) $(CMFLAGS)

.PHONY: $(PHONY)
	