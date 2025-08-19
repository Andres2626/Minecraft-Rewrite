#
# Makefile for libgfx dependency
# NOTE: To avoid installing the .py generator, I created a repository 
#       with the GLAD files. For more information, see include/glad.h
#

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
	
build-deps: install-deps make copy

make:
	$(MK) $(SRCDIR) all
	
copy:
	cp -r $(SRCDIR)/include/ ../ 
	@echo "copying all libgfx .$(LIBEXT) files"
	@for f in $(shell find $(OUTDIR) -name '*.$(LIBEXT)'); do \
		cp $$f $(LIBDIR)/; \
	done
	@echo "copying all libgfx .$(DYNEXT) files"
	@for f in $(shell find $(OUTDIR) -name '*.$(DYNEXT)'); do \
		cp $$f $(BINDIR)/; \
	done
	
clean-deps-install:
	rm -rf $(SRCDIR)
	rmdir $(SRCDIR)

clean-deps-build:
	$(MK) $(SRCDIR) clean
	
$(SRCDIR):
	@echo "installing GLAD source from $(REPO)"
	$(GIT) clone $(REPO)

.PHONY: $(PHONY)
	