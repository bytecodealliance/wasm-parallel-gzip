# Download and unpack wasi-sdk.

bin/clang-16: wasi-sdk-20.0.threads-linux.tar.gz
	tar --extract --verbose --touch --strip-components=1 --file=wasi-sdk-20.0.threads-linux.tar.gz

wasi-sdk-20.0.threads-linux.tar.gz:
	wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20%2Bthreads/$@
	touch $@

clean:
	rm -rf wasi-sdk-20.0.threads-linux.tar.gz wasi-sdk-20.0+threads
