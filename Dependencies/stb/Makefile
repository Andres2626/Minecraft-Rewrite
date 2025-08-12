#
# Makefile for stb dependency
# NOTE: The project at the moment only use stb_image
#
export MK = $(MAKE) -C
GIT         := git

# Remote repository
REPO 	:= https://github.com/nothings/stb.git

# Misc directories
SRCDIR	:= stb/
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY	+= install-deps build-deps clean-deps-install clean-deps-build
	
install-deps: $(SRCDIR)
	
build-deps: 
	@echo "Nothing to build"
	@echo "copying all STB includes"
	cp $(SRCDIR)/*.h $(INCDIR)
	
clean-deps-install:
	rm -rf $(SRCDIR)

clean-deps-build:
	@echo "Nothing to clean"

$(SRCDIR):
	@echo "installing STB source from $(REPO)"
	$(GIT) clone $(REPO)

.PHONY: $(PHONY)
	