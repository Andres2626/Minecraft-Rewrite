#
# Makefile for zlib dependency
# NOTE: At the moment the makefile only use winX (zlib/win32/makefile.gcc)
#

# Remote repository
REPO	:= https://github.com/madler/zlib.git

# Misc directories
SRCDIR  := zlib/
OUTDIR  := build/
BINDIR  := ../bin
INCDIR  := ../include
LIBDIR  := ../lib

PHONY   += install-deps build-deps clean-deps-install clean-deps-build
	
install-deps: $(SRCDIR)

# clean build dir
clean-deps-install:
	rm -rf $(SRCDIR)/

# clean install directory
clean-deps-build:
	rm -rf $(OUTDIR)/
	
build-deps: install-deps $(OUTDIR) 
	$(MK) $(OUTDIR) all
	@echo "copying all zlib include files"
	cp $(SRCDIR)/*.h $(INCDIR)
	
	@echo "copying all zlib .$(LIBEXT) files"
	@for f in $(shell find $(OUTDIR) -name '*.$(LIBEXT)'); do \
		cp $$f $(LIBDIR)/; \
	done
	
	@echo "copying all zlib .$(DYNEXT) files"
	@for f in $(shell find $(OUTDIR) -name '*.$(DYNEXT)'); do \
		cp $$f $(BINDIR)/; \
	done
	
$(SRCDIR):
	@echo "installing zlib source from $(REPO)"
	$(GIT) clone $(REPO)
	
$(OUTDIR):
	mkdir -p $@
	$(CMAKE) -G "Unix Makefiles" -S $(SRCDIR) -B $(OUTDIR) $(CMFLAGS)

.PHONY: $(PHONY)
	