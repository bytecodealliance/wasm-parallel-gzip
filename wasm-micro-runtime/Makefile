PLATFORM?=linux

WAMR_PLATFORM=linux
ifeq ($(PLATFORM), macos)
    WAMR_PLATFORM=darwin
endif

# Build WASM Micro Runtime.
build/iwasm:
	cd src && mkdir -p build && cd build && \
		cmake -DCMAKE_BUILD_TYPE=Release -DWAMR_BUILD_LIB_WASI_THREADS=1 ../product-mini/platforms/$(WAMR_PLATFORM)/ && \
		cmake --build .

# Note that we also need to clean up the Git tree as well.
clean:
	cd src && cmake --build build --target clean && git restore .
