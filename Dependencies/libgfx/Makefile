#
# Makefile for libgfx dependency
# NOTE: To avoid installing the .py generator, I created a repository 
#       with the GLAD files. For more information, see include/glad.h
#
export MK = $(MAKE) -C
GIT		:= git

# Remote repository
REPO	:= https://github.com/Andres2626/libgfx.git

# Misc directories
SRCDIR	:= libgfx/
OUTDIR  := $(SRCDIR)/build
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY   += install-deps build-deps clean-deps-install clean-deps-build
	
install-deps: $(SRCDIR)
	
build-deps: install-deps
	$(MK) $(SRCDIR) all
	cp -r $(SRCDIR)/include/ ../ 
	cp $(OUTDIR)/libgfx.dll $(BINDIR)/libgfx.dll
	cp $(OUTDIR)/libgfxdll.a $(LIBDIR)/libgfxdll.a
	
clean-deps-install:
	rm -rf $(SRCDIR)
	rmdir $(SRCDIR)

clean-deps-build:
	$(MK) $(SRCDIR) clean
	
$(SRCDIR):
	@echo "installing GLAD source from $(REPO)"
	$(GIT) clone $(REPO)

.PHONY: $(PHONY)
	