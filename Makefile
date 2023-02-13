# Build a WebAssembly benchmark for parallel compression and decompression (eventually).
PIGZ_WASM=pigz/pigz.wasm
ZLIB_WASM=zlib/libz.wasm.a
WASI_SDK_CC=wasi-sdk/bin/clang
WASMTIME=wasmtime/src/target/release/wasmtime

# Compile and link the various WebAssembly objects and the Wasmtime engine to run them. Important
# variables:
# - WASI_SDK_DIR: modify the location of the wasi-sdk version to something other than the retrieved
#   one
# - CC: modify the location of Clang used to build the artifacts, default: $WASI_SDK_DIR/bin/clang
# - DEBUG: when set, enables symbols everywhere (`-g`) and optional logging in pigz.
build: $(PIGZ_WASM) $(WASMTIME)
$(PIGZ_WASM): $(WASI_SDK_CC)
	make -C pigz
$(ZLIB_WASM): $(WASI_SDK_CC)
	make -C zlib
$(WASI_SDK_CC):
	make -C wasi-sdk
$(WASMTIME):
	make -C wasmtime

clean:
	make -C wasi-sdk clean
	make -C zlib clean
	make -C pigz clean
	make -C wasmtime clean
	rm -f random.*

# Run the pigz WebAssembly binary as a CLI application using Wasmtime; this is not meant to be a
# highly-accurate benchmarking setup but rather a quick check that things work. Because pigz will
# remove the original file, we keep an `.original.bin` copy around for replicatability.
NUM_THREADS ?= 1
benchmark: random.bin $(PIGZ_WASM) $(WASMTIME)
	@rm -f random.bin.gz
	time $(WASMTIME) --dir . --wasm-features=threads --wasi-modules=experimental-wasi-threads \
		$(PIGZ_WASM) -- -p $(NUM_THREADS) /random.bin
	@gzip --decompress --stdout random.bin.gz >/dev/null
random.bin: random.original.bin
	@cp $< $@
random.original.bin:
	head -c 100M </dev/urandom >$@
