# Generate `pigz.wasm`, a WebAssembly module for parallel gzip compression and decompression.

# Point to the retrieved version of wasi-sdk.
WASI_SDK_DIR ?= ../wasi-sdk

# Allow $(CC) to be set from the command line; ?= doesn't work for CC because make has a default
# value for it. Note the appended sysroot; see https://github.com/WebAssembly/wasi-sdk/#use.
ifeq ($(origin CC), default)
CC := $(WASI_SDK_DIR)/bin/clang --sysroot=$(WASI_SDK_DIR)/share/wasi-sysroot
endif

# Flags from the original Makefile.
CFLAGS = -O3 -Wall -Wextra -Wno-unknown-pragmas -Wcast-qual

# Include zlib's header files from the adjoining directory. Though a Linux system likely has zlib
# installed, since we already have the zlib source available (and to avoid incompatibility), we use
# the retrieved version.
CFLAGS += -I../zlib/src -Isrc

# We must explain to clang how to build a wasi-threads-enabled module. Note that the dual "import
# memory" then "export memory" linking is only necessary due to Wasmtime's Wiggle expectations and
# only really possible with LLVM 16.
CFLAGS += --target=wasm32-wasi-threads -pthread -mthread-model posix -ftls-model=local-exec
LDFLAGS = -Wl,--import-memory,--export-memory,--max-memory=67108864

# Pass DEBUG=1 to add symbols and enable debug logging (also must `-vv` from the command line).
ifdef DEBUG
CFLAGS += -g -DPIGZ_DEBUG
endif

SRC=src/
ZOPFLI_OBJS = deflate.wasm.o blocksplitter.wasm.o tree.wasm.o lz77.wasm.o cache.wasm.o hash.wasm.o \
	util.wasm.o squeeze.wasm.o katajainen.wasm.o symbols.wasm.o
ZOPFLI_SRC = $(SRC)zopfli/src/zopfli/
ZLIB_ARCHIVE=../zlib/libz.wasm.a

pigz.wasm: pigz.wasm.o yarn.wasm.o $(ZOPFLI_OBJS) $(ZLIB_ARCHIVE)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Note that we use a sibling project for a WebAssembly build of zlib.
$(ZLIB_ARCHIVE):
	make -C ../zlib

# The pigz Wasm object is modified slightly (see `pigz.diff`):
# - the original `pigz.c` file uses `try.h`, a `setjmp`-based macro header providing try-catch
#   functionality. Since `setjmp` and `longjmp` are not yet enabled in WebAssembly (ref.
#   exceptions), we replace `try.h` with a version that immediately fails instead of attempting to
#   catch errors -- `try-but-fail.h`.
# - we disable any signaling since support for this in Wasm (and wasi-libc) is experimental.
# - we remove any uses of `chown` and `chmod` until these are avilable in wasi-libc (TODO).
pigz.wasm.o: pigz.c $(SRC)yarn.h try-but-fail.h $(ZOPFLI_SRC)deflate.h $(ZOPFLI_SRC)util.h
	$(CC) $(CFLAGS) -c pigz.c -o $@
pigz.c: $(SRC)pigz.c patches/remove-signals.diff patches/use-try-but-fail.diff
	cp $< $@
	git apply --verbose --directory pigz --recount patches/remove-signals.diff \
		patches/use-try-but-fail.diff patches/remove-chown-chmod.diff

# The yarn Wasm object is also modified slightly: we remove the single use of
# `pthread_attr_setdetachstate` (likely redundant anyways) until it is enabled in wasi-libc (TODO).
yarn.wasm.o: yarn.c $(SRC)yarn.h
	$(CC) $(CFLAGS) -c $< -o $@
yarn.c: $(SRC)yarn.c patches/remove-setdetachstate.diff
	cp $< $@
	git apply --verbose --directory pigz --recount patches/remove-setdetachstate.diff

deflate.wasm.o: $(ZOPFLI_SRC)deflate.c $(ZOPFLI_SRC)deflate.h $(ZOPFLI_SRC)blocksplitter.h $(ZOPFLI_SRC)lz77.h $(ZOPFLI_SRC)squeeze.h $(ZOPFLI_SRC)tree.h $(ZOPFLI_SRC)zopfli.h $(ZOPFLI_SRC)cache.h $(ZOPFLI_SRC)hash.h $(ZOPFLI_SRC)util.h $(ZOPFLI_SRC)symbols.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)deflate.c -o $@

blocksplitter.wasm.o: $(ZOPFLI_SRC)blocksplitter.c $(ZOPFLI_SRC)blocksplitter.h $(ZOPFLI_SRC)deflate.h $(ZOPFLI_SRC)lz77.h $(ZOPFLI_SRC)squeeze.h $(ZOPFLI_SRC)tree.h $(ZOPFLI_SRC)util.h $(ZOPFLI_SRC)zopfli.h $(ZOPFLI_SRC)cache.h $(ZOPFLI_SRC)hash.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)blocksplitter.c -o $@

tree.wasm.o: $(ZOPFLI_SRC)tree.c $(ZOPFLI_SRC)tree.h $(ZOPFLI_SRC)katajainen.h $(ZOPFLI_SRC)util.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)tree.c -o $@

lz77.wasm.o: $(ZOPFLI_SRC)lz77.c $(ZOPFLI_SRC)lz77.h $(ZOPFLI_SRC)util.h $(ZOPFLI_SRC)cache.h $(ZOPFLI_SRC)hash.h $(ZOPFLI_SRC)zopfli.h $(ZOPFLI_SRC)symbols.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)lz77.c -o $@

cache.wasm.o: $(ZOPFLI_SRC)cache.c $(ZOPFLI_SRC)cache.h $(ZOPFLI_SRC)util.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)cache.c -o $@

hash.wasm.o: $(ZOPFLI_SRC)hash.c $(ZOPFLI_SRC)hash.h $(ZOPFLI_SRC)util.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)hash.c -o $@

util.wasm.o: $(ZOPFLI_SRC)util.c $(ZOPFLI_SRC)util.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)util.c -o $@

squeeze.wasm.o: $(ZOPFLI_SRC)squeeze.c $(ZOPFLI_SRC)squeeze.h $(ZOPFLI_SRC)blocksplitter.h $(ZOPFLI_SRC)deflate.h $(ZOPFLI_SRC)tree.h $(ZOPFLI_SRC)util.h $(ZOPFLI_SRC)zopfli.h $(ZOPFLI_SRC)lz77.h $(ZOPFLI_SRC)cache.h $(ZOPFLI_SRC)hash.h $(ZOPFLI_SRC)symbols.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)squeeze.c -o $@

katajainen.wasm.o: $(ZOPFLI_SRC)katajainen.c $(ZOPFLI_SRC)katajainen.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)katajainen.c -o $@

symbols.wasm.o: $(ZOPFLI_SRC)symbols.c $(ZOPFLI_SRC)symbols.h
	$(CC) $(CFLAGS) -c $(ZOPFLI_SRC)symbols.c -o $@

# For comparison purposes, it can also be nice to have a copy of the native version of the
# application.
pigz:
	make -C src pigz
	cp src/pigz $@

clean:
	rm -rf *.o *.c *.wasm pigz
