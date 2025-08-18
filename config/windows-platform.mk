export PLATFORM = windows
export DYNEXT = dll
export LIBEXT = a
export EXECEXT = exe

# detect compiler toolchain
ifneq (,$(findstring mingw,$(COMPILER_TARGET)))
TOOLCHAIN = mingw
EXEC_CFLAGS = -shared -Wl,--out-implib,
else ifneq (,$(findstring cygwin,$(COMPILER_TARGET)))
OUT_CFLAGS = -FPIC
EXEC_CFLAGS = -shared
TOOLCHAIN = cygwin
else
TOOLCHAIN = unknoun
$(error windows toolchain $(COMPILER_TARGET) not supported)
endif

export TOOLCHAIN
export EXEC_CFLAGS
export OUT_CFLAGS

