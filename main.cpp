#include <hls_math.h>
#include <ap_int.h>
#include "ldpcDec.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){
	int err_cnt = 0;
	ap_int<8> llr[N];
	bool gc_result[N];
	bool rtl_result[K];
	FILE *fp1,*fp2;
	fp1 = fopen("./data/llr.txt", "r");
	fp2 = fopen("./data/result.txt", "r");
	for(int i=0;i<N;i++){
		int d;
		int r;
		fscanf(fp1, "%d", &d);
		fscanf(fp2, "%d", &r);
		llr[i] = d;
		gc_result[i] = r==1;
	}
	ldpcDec(llr,rtl_result);
	ldpcDec(llr,rtl_result);
	for(int i=0;i<K;i++){
		if(rtl_result[i]!=gc_result[i])
			err_cnt++;
	}
	printf("Error cnt : %d\n",err_cnt);
	return err_cnt;
}
