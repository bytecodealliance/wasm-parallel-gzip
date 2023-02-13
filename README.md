wasm-parallel-gzip
===================

This project demonstrates how to build and run parallel compression using WebAssembly and
[wasi-threads]. It compiles the [pigz] CLI tool to WebAssembly using a [wasi-sdk] pre-release and
runs it using a recent version of [wasmtime]. At the moment, wasi-threads is still [experimental];
though its ABI is unstable, this demo motivates the need for spawning threads in WebAssembly.

[experimental]: https://github.com/WebAssembly/wasi-threads/issues/10
[wasi-threads]: https://github.com/WebAssembly/wasi-threads

### Prerequisites

```
git clone --recurse-submodules ...
```

The various source repositories are retrieved as Git submodules &mdash; these are all necessary for
the build to succeed. Also, this project expects some basic Linux tools (`make`, `wget`, `git`,
`tar`) as well as Rust. The specific Linux expectations are incidental to this sample code;
contributions are welcome to make this sample work on other OSes.

### Build

```
make
```

This will build or retrieve all of the components necessary for running the parallel compression
tool:
- [wasi-sdk]: an experimental [release] of the `wasi-sdk` toolchain with support for the
  `wasm32-wasi-threads` target
- [zlib]: the parallel compression library relies on this C library &mdash; we must build a
  WebAssembly version of it
- [pigz]: the parallel compression library
- [wasmtime]: a WebAssembly engine that implements wasi-threads

[pigz]: pigz
[wasi-sdk]: wasi-sdk
[wasmtime]: wasmtime
[zlib]: zlib
[release]: https://github.com/WebAssembly/wasi-sdk/releases/tag/wasi-sdk-20%2Bthreads

### Run

```
make benchmark
```

Run parallel compression (by default, `NUM_THREADS=1`) on a randomly-generated file.

### TODO

- add missing symbols to `wasi-libc`: `chown`, `chmod`, `pthread_attr_setdetachstate` (and remove
  from [pigz/patches](pigz/patches))

### License

This code sample is released under the [MIT](./LICENSE) license. By contributing to the project, you
agree to the license and copyright terms therein and release your contribution under these terms.
