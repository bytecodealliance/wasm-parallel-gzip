zlib (as WebAssembly)
=====================

This directory demonstrates how to build the [zlib] library to a WebAssembly object file. Since
dynamic linking is still only a remote thought in the WebAssembly ecosystem, this expects the object
file to be statically linked in.

[zlib]: https://github.com/madler/zlib

### Build

```shell
make
```

This should output a `libz.wasm.a` file containing all of the necessary compiled objects.
