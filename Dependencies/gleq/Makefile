#
# Makefile for GLEQ dependency
#

export MK = $(MAKE) -C
GIT         := git
			
# Remote GIT repository
REPO        := https://github.com/glfw/gleq.git

# Misc dirs
SRCDIR  := gleq/
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
	@echo "copying GLEQ includes"
	cp $(SRCDIR)/gleq.h $(INCDIR)/gleq.h
	
$(SRCDIR):
	@echo "installing GLFW source from $(REPO)"
	$(GIT) clone $(REPO)
	
$(OUTDIR):
	mkdir -p $@

.PHONY: $(PHONY)
	