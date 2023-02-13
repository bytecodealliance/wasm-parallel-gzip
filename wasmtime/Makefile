# Build Wasmtime.
target/release/wasmtime:
	cd src && cargo build --features=wasi-threads --release

# Note that we also need to clean up the Git tree as well.
clean:
	cd src && cargo clean && git restore .
