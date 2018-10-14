#ifndef LDPCDECH
#define LDPCDECH



#include <hls_math.h>
#include <ap_int.h>
#define N 1536
#define K 1024
#define BL 64
#define ITERNUM 50
struct ARRAY2{
	ap_int<8> a1,a2;
};
struct ARRAY12{
	ap_int<8> a[12];
};
const ap_uint<9> rowPara[8][10]={{0,3,15,18,27,30,39,42,63,66},
		                         {6,16,28,31,43,48,54,60,67,68},
								 {7,12,19,29,36,44,51,55,69,70},
								 {8,13,20,24,32,40,56,61,71,72},
								 {1,9,21,25,37,45,52,64,73,74},
								 {10,17,22,33,41,49,57,62,75,76},
								 {2,4,14,23,34,46,53,58,77,78},
								 {5,11,26,35,38,47,50,59,65,79}};
const ap_uint<16> pidx[80] = {3,20,35,0,25,6,1,12,19,6,10,6,2,24,37,2,36,
		28,0,15,3,10,20,21,0,29,4,3,34,40,7,10,6,8,5,14,3,28,30,1,17,
		36,1,18,15,14,0,3,2,9,36,2,38,4,3,13,8,21,20,14,0,39,45,1,0,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const ap_uint<8> sel[25] = {0,3,6,12,15,18,24,27,30,36,39,42,48,51,54,60,63,66,68,70,72,74,76,78,80};
void ldpcDec(ap_int<8> llr[N],bool output[K]);
inline ARRAY12 rowUpdate12(ARRAY12 a);
inline void colUpdate3(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512],ap_int<8> c[512]);
inline void colUpdate2(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512]);
inline void colUpdate6(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512],ap_int<8> c[512],ap_int<8> d[512],ap_int<8> e[512],ap_int<8> f[512]);

#endif
