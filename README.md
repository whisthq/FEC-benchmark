# Whist FEC Benchmark

A tiny FEC test and benchmark framework, to help you verify if the FEC libs work, and for comparing their performance. Each folder contains a specific open-source FEC library, and the `common.h` file contains the parameters used for testing their
respective performance in a controlled manner.

## Compiling

### Linux (x64)

`make -j OPT=-mavx2` or `make -j OPT=-mssse3`.

`make -j` doesn't compile, you need to specify the `OPT` flag.

##### Note:

`make -j OPT=-mavx2` enables `avx2` (at compile time), which is the fastest setting.

`make -j OPT=-mssse3` enables `sse3` but not `avx2`

### MacOS (x64)

`make -j` or `make -j OPT=-mssse3` or `make -j OPT=-mavx2`

##### Note:

`make -j OPT=-mavx2` enables avx2, which is the fastest setting.

`make -j` and `make -j OPT=-mssse3` both enable sse3 but not avx2, and have the same speed.

### macOS (arm64)

This needs to be figured out. For now, we mostly do benchmarking on x64 systems, and assume that the arm64 version will follow the same performance ordering between the libraries.

## Compile Output

* `lugi.out`       Our current RS lib
* `zfec.out`       zfec's RS
* `leopard.out`    NlogN RS 
* `longhair.out`   https://github.com/catid/longhair
* `cm256.out`      https://github.com/catid/cm256
* `wirehair.out`   O(N) fountain code    https://github.com/catid/wirehair
* `fecal.out`      O(N^2) founain code, faster for smaller case    https://github.com/catid/fecal
* `mem.out`        Not FEC, just benchmark malloc, memcpy, free. Help you get an idea of how costy those FEC codes are.

run each to test if it works and get the performance


## Changing Test Parameters

You can change the parameters in `common.h` and recompile using the Makefile commands listed above.
