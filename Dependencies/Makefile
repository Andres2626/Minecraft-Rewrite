SUBDIRS += libgfx GLFW gleq glm gzip-hpp stb zlib
export MK = $(MAKE) -C
PHONY   := all clean distclean install-deps build-deps clean-deps-install clean-deps-build

BINDIR  := bin/
LIBDIR  := lib/
INCDIR  := include/
DIRS    += $(BINDIR) $(LIBDIR) $(INCDIR)

all: install-deps build-deps
	
clean: clean-deps-build clean-deps-install
	rmdir $(BINDIR)
	rmdir $(LIBDIR)
	rmdir $(INCDIR)
	
install-deps: $(SUBDIRS)
	@echo "installing dependencies from remote repository"
	for dir in $(SUBDIRS); do \
		$(MK) $$dir $@; \
	done

build-deps: $(DIRS) $(SUBDIRS)
	@echo "building all dependencies from source"
	for dir in $(SUBDIRS); do \
		$(MK) $$dir $@; \
	done

clean-deps-install:$(SUBDIRS)
	@echo "cleaning dependencies build files"
	for dir in $(SUBDIRS); do \
		$(MK) $$dir $@; \
	done

clean-deps-build:$(SUBDIRS)
	rm -rf $(BINDIR)/*
	rm -rf $(LIBDIR)/*
	rm -rf $(INCDIR)/*
	rmdir $(BINDIR) $(LIBDIR) $(INCDIR)
	@echo "cleaning all dependencies files"
	for dir in $(SUBDIRS); do \
		$(MK) $$dir $@; \
	done

# copy all builded binaries, libraries and includes to local directories
$(BINDIR):
	mkdir -p $@
	
$(INCDIR):
	mkdir -p $@

$(LIBDIR):
	mkdir -p $@

.PHONY: $(PHONY)
	