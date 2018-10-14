
#include <hls_math.h>
#include <ap_int.h>
#include "ldpcDec.h"
void readData(ap_int<8> llr[N],ap_int<8> minfo[80][512],ap_int<8> blockllr[24][512]){
	for(int i=0;i<24;i++){
		for(int j=0;j<BL;j++){
#pragma HLS PIPELINE
			ap_int<8> s = llr[i*BL+j];
			blockllr[i][j] = s;
			parall_assign:for(int k=0;k<80;k++){
			#pragma HLS UNROLL factor=80
				if(k>=sel[i]&&k<sel[i+1])
					minfo[k][j] = s;
			}
		}
	}

}

void updateMinfo(ap_int<8> llr[N], ap_int<8> minfo[80][512],ap_int<8> blockllr[24][512],bool blockdout[24][512]){

#pragma HLS ARRAY_PARTITION variable=minfo complete dim=1
#pragma HLS ARRAY_PARTITION variable=blockllr complete dim=1
#pragma HLS ARRAY_PARTITION variable=blockdout complete dim=1
	for(int i=0;i<BL;i++){
#pragma HLS DEPENDENCE variable=minfo inter false
#pragma HLS PIPELINE
		ARRAY12 a[8];
		ARRAY12 b[8];
		for(int j=0;j<8;j++){
#pragma HLS UNROLL
			for(int k=0;k<12;k++){
#pragma HLS UNROLL
				if(k<10)
					a[j].a[k] = minfo[rowPara[j][k]][(pidx[rowPara[j][k]]+i)%BL];
				else
					a[j].a[k] = 127;
			}
			b[j] = rowUpdate12(a[j]);
			for(int k=0;k<10;k++){
#pragma HLS UNROLL
				minfo[rowPara[j][k]][(pidx[rowPara[j][k]]+i)%BL] = b[j].a[k];
			}
		}
	}
	colUpdate3(blockdout[0],blockllr[0],minfo[0],minfo[1],minfo[2]);
	colUpdate3(blockdout[1],blockllr[1],minfo[3],minfo[4],minfo[5]);
	colUpdate6(blockdout[2],blockllr[2],minfo[6],minfo[7],minfo[8],minfo[9],minfo[10],minfo[11]);
	colUpdate3(blockdout[3],blockllr[3],minfo[12],minfo[13],minfo[14]);
	colUpdate3(blockdout[4],blockllr[4],minfo[15],minfo[16],minfo[17]);
	colUpdate6(blockdout[5],blockllr[5],minfo[18],minfo[19],minfo[20],minfo[21],minfo[22],minfo[23]);
	colUpdate3(blockdout[6],blockllr[6],minfo[24],minfo[25],minfo[26]);
	colUpdate3(blockdout[7],blockllr[7],minfo[27],minfo[28],minfo[29]);
	colUpdate6(blockdout[8],blockllr[8],minfo[30],minfo[31],minfo[32],minfo[33],minfo[34],minfo[35]);
	colUpdate3(blockdout[9],blockllr[9],minfo[36],minfo[37],minfo[38]);
	colUpdate3(blockdout[10],blockllr[10],minfo[39],minfo[40],minfo[41]);
	colUpdate6(blockdout[11],blockllr[11],minfo[42],minfo[43],minfo[44],minfo[45],minfo[46],minfo[47]);
	colUpdate3(blockdout[12],blockllr[12],minfo[48],minfo[49],minfo[50]);
	colUpdate3(blockdout[13],blockllr[13],minfo[51],minfo[52],minfo[53]);
	colUpdate6(blockdout[14],blockllr[14],minfo[54],minfo[55],minfo[56],minfo[57],minfo[58],minfo[59]);
	colUpdate3(blockdout[15],blockllr[15],minfo[60],minfo[61],minfo[62]);
	colUpdate3(blockdout[16],blockllr[16],minfo[63],minfo[64],minfo[65]);
	colUpdate2(blockdout[17],blockllr[17],minfo[66],minfo[67]);
	colUpdate2(blockdout[18],blockllr[18],minfo[68],minfo[69]);
	colUpdate2(blockdout[19],blockllr[19],minfo[70],minfo[71]);
	colUpdate2(blockdout[20],blockllr[20],minfo[72],minfo[73]);
	colUpdate2(blockdout[21],blockllr[21],minfo[74],minfo[75]);
	colUpdate2(blockdout[22],blockllr[22],minfo[76],minfo[77]);
	colUpdate2(blockdout[23],blockllr[23],minfo[78],minfo[79]);
}


void ldpcDec(ap_int<8> llr[N],bool output[K]){
#pragma HLS INTERFACE axis register both port=llr
#pragma HLS INTERFACE axis register both port=output
	   ap_int<8> minfo[80][512];
	   ap_int<8> blockllr[24][512];
	   bool blockdout[24][512];
	   readData(llr,minfo,blockllr);
	   for(int i=0;i<ITERNUM;i++)
		   updateMinfo(llr,minfo,blockllr,blockdout);
	   int cnt = 0;
	   for(int i=0;i<16;i++){
		   for(int j=0;j<BL;j++){
#pragma HLS PIPELINE
			   output[cnt] = blockdout[i][j];
			   cnt++;
		   }
	   }

}



inline ARRAY2 compMin(ap_int<8> a,ap_int<8> b,ap_int<8> c,ap_int<8> d){
	ARRAY2 res;
	if(a<c){
		res.a1 = a;
		if(b<c)
			res.a2 = b;
		else
			res.a2 = c;
	}
	else{
		res.a1 = c;
		if(a<d)
			res.a2 = a;
		else
			res.a2 = d;
	}
	return res;
}
inline ARRAY12 rowUpdate12(ARRAY12 a){
#pragma HLS PIPELINE
#pragma HLS ARRAY_PARTITION variable=a complete dim=1
	bool mark1[12];
	ap_int<8> a1[12];
	for(int i=0;i<12;i++){
#pragma HLS UNROLL
		if(a.a[i]<0){
			mark1[i] = false;
			a1[i] =  (-a.a[i])-((-a.a[i])>>3)-((-a.a[i])>>4);
		}
		else{
			mark1[i] = true;
			a1[i] =  a.a[i] - (a.a[i]>>3) - (a.a[i]>>4);
		}
	}
	ap_int<8> a2[12];
	for(int i=0;i<6;i++){
#pragma HLS UNROLL
		if(a1[2*i]<a1[2*i+1]){
			a2[2*i] = a1[2*i];
			a2[2*i+1] = a1[2*i+1];
		}
		else{
			a2[2*i] = a1[2*i+1];
			a2[2*i+1] = a1[2*i];
		}
	}
	ARRAY2 a3[2];
	a3[0] = compMin(a2[0],a2[1],a2[2],a2[3]);
	a3[1] = compMin(a2[4],a2[5],a2[6],a2[7]);
	ARRAY2 a4[2];
	a4[0] = compMin(a3[0].a1,a3[0].a2,a3[1].a1,a3[1].a2);
	a4[1] = compMin(a2[8],a2[9],a2[10],a2[11]);
	ARRAY2 a5;
	a5 = compMin(a4[0].a1,a4[0].a2,a4[1].a1,a4[1].a2);
	bool marka = true;
	for(int i=0;i<12;i++){
#pragma HLS UNROLL
		marka = marka==mark1[i];
	}
	ARRAY12 res;
#pragma HLS ARRAY_PARTITION variable=res complete dim=1
	for(int i=0;i<12;i++){
#pragma HLS UNROLL
		if(marka==mark1[i]){
			if(a5.a1==a1[i])
				res.a[i] = a5.a2;
			else
				res.a[i] = a5.a1;
		}
		else{
			if(a5.a1==a1[i])
				res.a[i] = -a5.a2;
			else
				res.a[i] = -a5.a1;
		}
	}
	return res;
}

inline void colUpdate3(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512],ap_int<8> c[512]){
	for(int i=0;i<BL;i++){
#pragma HLS PIPELINE
		ap_int<8> t[3];
		t[0] = a[i];
		t[1] = b[i];
		t[2] = c[i];
		ap_int<11> sum = l[i] + t[0] + t[1] + t[2];
		ap_int<11> s[3];
#pragma UNROLL
		for(int j=0;j<3;j++){
			s[j] = sum - t[j];
			if(s[j]>127)
				s[j] = 127;
			if(s[j]<-128)
				s[j] = -128;
		}
		a[i] = s[0];
		b[i] = s[1];
		c[i] = s[2];
		r[i] = sum<0;
	}
}
inline void colUpdate2(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512]){
	for(int i=0;i<BL;i++){
	#pragma HLS PIPELINE
			ap_int<8> t[2];
			t[0] = a[i];
			t[1] = b[i];
			ap_int<11> sum = l[i] + t[0] + t[1];
			ap_int<11> s[2];
	#pragma UNROLL
			for(int j=0;j<2;j++){
				s[j] = sum - t[j];
				if(s[j]>127)
					s[j] = 127;
				if(s[j]<-128)
					s[j] = -128;
			}
			a[i] = s[0];
			b[i] = s[1];
			r[i] = sum<0;
		}
}
inline void colUpdate6(bool r[512],ap_int<8> l[512],ap_int<8> a[512],ap_int<8> b[512],ap_int<8> c[512],ap_int<8> d[512],ap_int<8> e[512],ap_int<8> f[512]){
	for(int i=0;i<BL;i++){
	#pragma HLS PIPELINE
			ap_int<8> t[6];
			t[0] = a[i];
			t[1] = b[i];
			t[2] = c[i];
			t[3] = d[i];
			t[4] = e[i];
			t[5] = f[i];
			ap_int<11> sum = l[i] + t[0] + t[1] + t[2] + t[3] + t[4] + t[5];
			ap_int<11> s[6];
	#pragma UNROLL
			for(int j=0;j<6;j++){
				s[j] = sum - t[j];
				if(s[j]>127)
					s[j] = 127;
				if(s[j]<-128)
					s[j] = -128;
			}
			a[i] = s[0];
			b[i] = s[1];
			c[i] = s[2];
			d[i] = s[3];
			e[i] = s[4];
			f[i] = s[5];
			r[i] = sum<0;
		}
}

