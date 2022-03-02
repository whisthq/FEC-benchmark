FLAGS=  -O2 -ggdb -I . -lstdc++  --std=c++11 #-Wno-stringop-overflow

all: wh.out fecal.out lugi.out zfec.out cm256.out lh.out leo.out mem.out

#benchmarks for MDS codes, currenly all of them are RS code
lugi.out: common.h
	gcc -o lugi.out -D LUGI mds_fec_bench.cpp rs/rs.cpp   ${FLAGS} ${OPT}
zfec.out: common.h
	gcc -o zfec.out -D ZFEC mds_fec_bench.cpp zfec/zfec.c  ${FLAGS} ${OPT}
cm256.out: common.h
	gcc -o cm256.out -D CM256 mds_fec_bench.cpp cm256/cm256.cpp cm256/gf256.cpp  ${FLAGS} ${OPT}
lh.out: common.h
	gcc -o lh.out -D LH mds_fec_bench.cpp longhair/gf256.cpp longhair/cauchy_256.cpp  ${FLAGS} ${OPT}
leo.out: common.h
	gcc -o leo.out -D LEO mds_fec_bench.cpp leopard/*.cpp  ${FLAGS} ${OPT}

#benchmarks for non-mds codes, e.g. fountain codes
wh.out: common.h
	gcc -o wh.out -D WH non_mds_fec_bench.cpp wirehair/*.cpp  ${FLAGS} ${OPT}
fecal.out: common.h
	gcc -o fecal.out -D FECAL non_mds_fec_bench.cpp fecal/*.cpp  ${FLAGS} ${OPT}

#benchmark of malloc/memcpy/free
mem.out: common.h
	gcc -o mem.out mem_bench.cpp ${FLAGS}

clean:
	rm -f *.out
