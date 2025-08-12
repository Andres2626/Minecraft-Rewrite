#
# Makefile for zlib dependency
# NOTE: At the moment the makefile only use winX (zlib/win32/makefile.gcc)
#
export MK = $(MAKE)
GIT		:= git

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
	
build-deps: install-deps $(OUTDIR)
	@echo "copying all ZLIB files"
	cp $(SRCDIR)/*.h $(INCDIR)
	cp $(OUTDIR)/libz.a $(LIBDIR)/libz.a
	cp $(OUTDIR)/libz.dll.a $(LIBDIR)/libz.dll.a
	cp $(OUTDIR)/zlib1.dll $(BINDIR)/zlib1.dll
	
clean-deps-install:
	rm -rf $(SRCDIR)

clean-deps-build:
	rm -rf $(OUTDIR)/zlib1.dll
	rm -rf $(OUTDIR)/libz.dll.a
	rm -rf $(OUTDIR)/libz.a
	rmdir $(OUTDIR)
	cd $(SRCDIR) && $(MK) -f win32/makefile.gcc clean

$(OUTDIR):
	mkdir -p $@
	cd $(SRCDIR) && $(MK) -f win32/makefile.gcc all
	cp $(SRCDIR)/zlib1.dll $(OUTDIR)/zlib1.dll
	cp $(SRCDIR)/libz.dll.a $(OUTDIR)/libz.dll.a
	cp $(SRCDIR)/libz.a $(OUTDIR)/libz.a

$(SRCDIR):
	@echo "installing zlib source from $(REPO)"
	$(GIT) clone $(REPO)

.PHONY: $(PHONY)
	