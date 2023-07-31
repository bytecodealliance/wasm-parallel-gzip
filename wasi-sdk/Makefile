# Download and unpack wasi-sdk.
TAR=tar
ifeq ($(PLATFORM), macos)
    TAR=gtar
endif

bin/clang-16: wasi-sdk-20.0-$(PLATFORM).tar.gz
	$(TAR) --extract --verbose --touch --strip-components=1 --file=wasi-sdk-20.0-$(PLATFORM).tar.gz

wasi-sdk-20.0-linux.tar.gz:
	wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/$@
	touch $@

wasi-sdk-20.0-macos.tar.gz:
	wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-20/$@
	touch $@

clean:
	rm -rf wasi-sdk-20.0-$(PLATFORM).tar.gz wasi-sdk-20.0
