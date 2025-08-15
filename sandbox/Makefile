#
# Makefile for sandbox
#

# Misc directories
TOPDIR 			:= ..
INCDIR 			:= $(TOPDIR)/include
OUTDIR 			:= $(TOPDIR)/build
DEPDIR 			:= $(TOPDIR)/Dependencies
OUTPUT_LIB		:= $(OUTDIR)/lib
OUTPUT_BIN		:= $(OUTDIR)/bin
OUTPUT_INC 		:= $(OUTDIR)/include
DEP_LIB			:= $(DEPDIR)/lib
DEP_BIN			:= $(DEPDIR)/bin
DEP_INC			:= $(DEPDIR)/include

EXEC        := $(OUTPUT_BIN)/rd-132211

ifeq ($(TOOLCHAIN), mingw)
LIB    := $(OUTPUT_LIB)/libmccoredll.$(LIBEXT)
LIBLNK := -lopengl32 -lgfxdll -lglfw3dll -lz 
else ifeq ($(TOOLCHAIN), cygwin)
LIB    :=
LIBLNK := -lopengl32 -lgfx -lglfw-3 -lz 
endif

LFLAGS += -L$(DEP_LIB) -L$(OUTPUT_BIN) -L$(DEP_BIN)
IFLAGS += -I$(INCDIR) -I$(INCDIR)/core -I$(INCDIR)/sandbox/ -I$(DEP_INC) -I$(OUTPUT_INC)
CFLAGS += $(LFLAGS) $(IFLAGS)
CPPFILES+= $(shell find -name "*.cpp")
OFILES += $(patsubst %.cpp, $(OUTDIR)/sandbox/%.o, $(CPPFILES))
DFILES := $(patsubst %.o, %.d, $(OFILES))

EXTRA_CFLAGS += $(LFLAGS) $(LIBLNK) -lmccore

PHONY += all clean distclean
.PHONY: $(PHONY)

all: $(OUTPUT_BIN) $(OUTPUT_LIB) $(OUTPUT_INC) $(EXEC)

distclean: clean
	
clean: 
	rm -rf $(OFILES)
	rm -rf $(DFILES)
	rm -rf $(EXEC)
	rm -rf $(OUTDIR)/sandbox/*
	rmdir $(OUTDIR)/sandbox

# include all dependencies
-include $(DFILES)

$(EXEC): $(OFILES)
	@mkdir -p $(@D)
	$(CPP) $^ -o $@ $(EXTRA_CFLAGS)

$(OUTDIR)/sandbox/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) -c $< -o $@ $(CFLAGS)

$(OUTPUT_INC):
	@mkdir -p $@

$(OUTPUT_LIB):
	@mkdir -p $@
	
$(OUTPUT_BIN):
	@mkdir -p $@
