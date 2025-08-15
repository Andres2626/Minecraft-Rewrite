#
# Makefile for gzip-hpp dependency
#

# Remote repository
REPO        := https://github.com/mapbox/gzip-hpp.git

# Misc directories
SRCDIR	:= gzip-hpp/
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY   += install-deps build-deps clean-deps-install clean-deps-build
	
install-deps: $(SRCDIR)
	
build-deps: install-deps $(INCDIR)/gzip
	@echo "Nothing to build"
	
clean-deps-install:
	rm -rf $(SRCDIR)

clean-deps-build:
	@echo "Nothing to clean"
	
$(INCDIR)/gzip: 
	@echo "copying all gzip-hpp include libs"
	cp -r $(SRCDIR)/include/gzip $(INCDIR)/

$(SRCDIR):
	@echo "installing gzip-hpp source from $(REPO)"
	$(GIT) clone $(REPO)
	
.PHONY: $(PHONY)
	