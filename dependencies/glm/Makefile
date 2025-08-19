#
# Makefile for GLM dependency
#

# Remote repository
REPO	:= https://github.com/icaven/glm.git

# Misc dirs
SRCDIR	:= glm/
OUTDIR  := build/
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY	+= install-deps build-deps clean-deps-install clean-deps-build
	
install-deps: $(SRCDIR)
	
build-deps: install-deps $(INCDIR)/glm
	@echo "Nothing to build"
	
clean-deps-install:
	rm -rf $(SRCDIR)

clean-deps-build:
	@echo "Nothing to clean"

$(INCDIR)/glm:
	@echo "copying all GLM include libs"
	cp -r $(SRCDIR)/glm $(INCDIR)/

$(OUTDIR):
	mkdir -p $@
	
$(SRCDIR):
	@echo "installing GLM source from $(REPO)"
	$(GIT) clone $(REPO)

.PHONY: $(PHONY)
	