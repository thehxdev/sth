# STH
sth is a base layer for my C projects. For now it provides a thread-safe memory
allocation tracer and abstractions to choose between libc malloc and [MiMalloc](https://github.com/microsoft/mimalloc).

> [!WARNING]
> This project is still pre-alpha stage. Use it at your own risk.

## Usage
This project is mostly for personal usage and fits how I write code. I consider that
You'll be using this library as a submodule or a raw dependency that will be built
and linked with your source code. **For now** the library is small and does not have
any external dependencies beside MiMalloc so using it as a compiled shared/static library
may not be a big of a deal but that's not how I use it and I'm not promise that this will
be the case in the future.

All the public headers are in [include/sth](include/sth) directory.
