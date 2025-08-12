#
# Makefile for MC Engine
#
export MK = $(MAKE) -C
CPP     := g++
CC		:= gcc
AR 		:= ar
NM      := nm

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
LIB    := $(OUTPUT_LIB)/libmccoredll.a

LFLAGS += -L$(DEP_LIB) -L$(OUTPUT_LIB)
IFLAGS += -I$(INCDIR) -I$(INCDIR)/core -I$(INCDIR)/sandbox/ -I$(DEP_INC) -I$(OUTPUT_INC)
CFLAGS += $(IFLAGS) -FPIC -DMC_EXPORT_DLL -MMD -MP

EXEC_CFLAGS += -Wl,--out-implib,$(LIB) $(LFLAGS) -lopengl32 -lgfxdll -lglfw3dll -lz 

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
	$(CPP) -shared $^ -o $@ $(EXEC_CFLAGS)

$(OUTDIR)/core/%.o: %.cpp
	@mkdir -p $(@D)
	$(CPP) -c $< -o $@ $(CFLAGS)