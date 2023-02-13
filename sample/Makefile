# Build and run the blog post sample code.

# Point to the retrieved version of wasi-sdk.
WASI_SDK_DIR ?= ../wasi-sdk

# Run the sample with the pre-built version of Wasmtime (see `make -C wasmtime`).
run: threads.wasm
	../wasmtime/src/target/release/wasmtime run \
		--wasm-features=threads --wasi-modules=experimental-wasi-threads \
		threads.wasm

# Compile the sample with the retrieved version of wasi-sdk (see `make -C wasi-sdk`).
threads.wasm: threads.c
	$(WASI_SDK_DIR)/bin/clang --sysroot $(WASI_SDK_DIR)/share/wasi-sysroot \
    	--target=wasm32-wasi-threads -pthread \
    	-Wl,--import-memory,--export-memory,--max-memory=67108864 \
    	threads.c -o threads.wasm

clean:
	rm threads.wasm
