# Misc directories
TOPDIR := ..
INCDIR := $(TOPDIR)/include
OUTDIR := $(TOPDIR)/build
DEPDIR := $(TOPDIR)/Dependencies
OUTPUT_LIB := $(OUTDIR)/lib
OUTPUT_BIN := $(OUTDIR)/bin
OUTPUT_INC := $(OUTDIR)/include
DEP_LIB		:= $(DEPDIR)/lib
DEP_BIN		:= $(DEPDIR)/bin
DEP_INC		:= $(DEPDIR)/include

BIN    := $(OUTPUT_BIN)/mccore.dll

ifeq ($(TOOLCHAIN), mingw)
LIB    := $(OUTPUT_LIB)/libmccoredll.$(LIBEXT)
LIBLNK := -lopengl32 -lgfxdll -lglfw3dll -lz
else ifeq ($(TOOLCHAIN), cygwin)
LIB    :=
LIBLNK := -lopengl32 -lgfx -lglfw-3 -lz
endif

LFLAGS += -L$(DEP_LIB) -L$(OUTPUT_LIB) -L$(DEP_BIN)
IFLAGS += -I$(INCDIR) -I$(INCDIR)/core -I$(INCDIR)/sandbox/ -I$(DEP_INC) -I$(OUTPUT_INC)
CFLAGS += $(IFLAGS) -FPIC -DMC_EXPORT_DLL -MMD -MP

EXTRA_CFLAGS += $(LFLAGS) $(LIBLNK)

EXCCPPFILES := ./Host/Windows/console.cpp ./Host/Windows/init.cpp
ALLCPPFILES := $(shell find . -name "*.cpp")
CPPFILES	:= $(filter-out $(EXCCPPFILES), $(ALLCPPFILES))
OFILES := $(patsubst %.cpp, $(OUTDIR)/core/%.o, $(CPPFILES))
DFILES := $(patsubst %.o, %.d, $(OFILES))

PHONY += all clean distclean
.PHONY: $(PHONY)

all: $(OUTPUT_BIN) $(OUTPUT_LIB) $(OUTPUT_INC) $(BIN)

clean:
	rm -rf $(OFILES)
	rm -rf $(DFILES)
	rm -rf $(EXEC)
	rm -rf $(OUTDIR)/core/*
	rmdir $(OUTDIR)/core

# include all dependencies
-include $(DFILES)

$(OUTPUT_INC):
	@mkdir -p $@

$(OUTPUT_LIB):
	@mkdir -p $@
	
$(OUTPUT_BIN):
	@mkdir -p $@

$(BIN): $(OFILES)
	@mkdir -p $(@D)
	$(CPP) -shared $^ -o $@ $(EXTRA_CFLAGS) $(EXEC_CFLAGS)$(LIB)

$(OUTDIR)/core/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) -c $< -o $@ $(CFLAGS)