# fec_bench
a tiny fec test and benmark framework, help you veryfiy if the fec libs work, and compare their performance
## How to compile
### Compile on linux X64

`make -j OPT=-mavx2` or `make -j OPT=-mssse3`.

`make -j` doesn't compile

##### Note:

`make -j OPT=-mavx2` enables avx2 (at compile time) , fastest  

`make -j OPT=-mssse3` enables sse3 but not avx2

### Compile on MacOS X64

`make -j` or `make -j OPT=-mssse3` or `make -j OPT=-mavx2`

##### Note:

`make -j OPT=-mavx2` enables avx2, fastest

`make -j` and `make -j OPT=-mssse3`  both enables sse3 but not avx2, and have same speed

### Compile on M1

need to figure out

## Compile Output


* `lugi.out`       our current RS lib
* `zfec.out`       zfec's RS
* `leopard.out`    NlogN RS 
* `longhair.out`   https://github.com/catid/longhair
* `cm256.out`      https://github.com/catid/cm256
* `wirehair.out`   O(N) fountain code    https://github.com/catid/wirehair
* `fecal.out`      O(N^2) founain code, faster for smaller case    https://github.com/catid/fecal
* `mem.out`        not FEC,  just benchmark malloc, memcpy, free. Help you get an idea of how costy those FEC codes are.

run each to test if it works and get the performance


## Change test parameters

change the parameters in `common.h` and recompile
