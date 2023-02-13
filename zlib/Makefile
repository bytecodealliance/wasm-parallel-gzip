# Generate `libz.wasm.a`, an archive containing zlib's compiled WebAssembly objects.
SHELL=/bin/sh

# Point to the retrieved version of wasi-sdk.
WASI_SDK_DIR ?= ../wasi-sdk

# Use the wasi-sdk versions of tools for archive creation. E.g., only llvm-ranlib can properly index
# a WebAssembly archive (?); see https://github.com/emscripten-core/emscripten/issues/9705.
AR=$(WASI_SDK_DIR)/bin/ar
ARFLAGS=rc
RANLIB=$(WASI_SDK_DIR)/bin/llvm-ranlib

# Allow $(CC) to be set from the command line; ?= doesn't work for CC because make has a default
# value for it. Note the appended sysroot; see https://github.com/WebAssembly/wasi-sdk/#use.
ifeq ($(origin CC), default)
CC := $(WASI_SDK_DIR)/bin/clang --sysroot=$(WASI_SDK_DIR)/share/wasi-sysroot
endif

# Flags from the original `./configure`-d Makefile (see original Makefile for more possible
# configurations).
CFLAGS = -O3 -D_LARGEFILE64_SOURCE=1 -DHAVE_HIDDEN

# We must explain to clang how to build a wasi-threads-enabled module.
CFLAGS += --target=wasm32-wasi-threads -pthread -mthread-model posix -ftls-model=local-exec
LDFLAGS ?= -Wl,--no-entry
# Eventually: LDFLAGS += -Wl,--import-memory,--export-memory,--max-memory=1048576

# Pass DEBUG=1 to add symbols and enable debug logging (also must `-vv` from the command line).
ifdef DEBUG
CFLAGS += -g
endif

SRCDIR=src/
OBJZ = adler32.wasm.o crc32.wasm.o deflate.wasm.o infback.wasm.o inffast.wasm.o inflate.wasm.o \
	inftrees.wasm.o trees.wasm.o zutil.wasm.o
OBJG = compress.wasm.o uncompr.wasm.o gzclose.wasm.o gzlib.wasm.o gzread.wasm.o gzwrite.wasm.o
OBJS = $(OBJZ) $(OBJG)

# Archive all of the zlib Wasm objects together.
libz.wasm.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
	($(RANLIB) $@ || true) >/dev/null 2>&1

# TODO: remove?
libz.wasm: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

zconf.h: $(SRCDIR)zconf.h.in
	cp $< $@

adler32.wasm.o: $(SRCDIR)adler32.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)adler32.c

crc32.wasm.o: $(SRCDIR)crc32.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)crc32.c

deflate.wasm.o: $(SRCDIR)deflate.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)deflate.c

infback.wasm.o: $(SRCDIR)infback.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)infback.c

inffast.wasm.o: $(SRCDIR)inffast.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)inffast.c

inflate.wasm.o: $(SRCDIR)inflate.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)inflate.c

inftrees.wasm.o: $(SRCDIR)inftrees.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)inftrees.c

trees.wasm.o: $(SRCDIR)trees.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)trees.c

zutil.wasm.o: $(SRCDIR)zutil.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)zutil.c

compress.wasm.o: $(SRCDIR)compress.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)compress.c

uncompr.wasm.o: $(SRCDIR)uncompr.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)uncompr.c

gzclose.wasm.o: $(SRCDIR)gzclose.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)gzclose.c

gzlib.wasm.o: $(SRCDIR)gzlib.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)gzlib.c

gzread.wasm.o: $(SRCDIR)gzread.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)gzread.c

gzwrite.wasm.o: $(SRCDIR)gzwrite.c
	$(CC) $(CFLAGS) -c -o $@ $(SRCDIR)gzwrite.c

# Lightly-adapted .h file dependencies for each object:
adler32.wasm.o zutil.wasm.o: $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h
gzclose.wasm.o gzlib.wasm.o gzread.wasm.o gzwrite.wasm.o: $(SRCDIR)zlib.h zconf.h $(SRCDIR)gzguts.h
compress.wasm.o example.wasm.o minigzip.wasm.o uncompr.wasm.o: $(SRCDIR)zlib.h zconf.h
crc32.wasm.o: $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h $(SRCDIR)crc32.h
deflate.wasm.o: $(SRCDIR)deflate.h $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h
infback.wasm.o inflate.wasm.o: $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h $(SRCDIR)inftrees.h $(SRCDIR)inflate.h $(SRCDIR)inffast.h $(SRCDIR)inffixed.h
inffast.wasm.o: $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h $(SRCDIR)inftrees.h $(SRCDIR)inflate.h $(SRCDIR)inffast.h
inftrees.wasm.o: $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h $(SRCDIR)inftrees.h
trees.wasm.o: $(SRCDIR)deflate.h $(SRCDIR)zutil.h $(SRCDIR)zlib.h zconf.h $(SRCDIR)trees.h

clean:
	rm -f *.o *.a *.h *.in
