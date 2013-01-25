#include  <stdlib.h>
#include  <assert.h>
#include "tdc.h"

int main(){

	// matriz exemplo
	unsigned char temp[] = { 140, 144, 147, 140, 140, 155, 179, 175,
	144, 152, 140, 147, 140, 148, 167, 179,
	152, 155, 136, 167, 163, 162, 152, 172,
	168, 145, 156, 160, 152, 155, 136, 160,
	162, 148, 156, 148, 140, 136, 147, 162,
	147, 167, 140, 155, 155, 140, 136, 162,
	136, 156, 123, 167, 162, 144, 140, 147,
	148, 155, 136, 155, 152, 147, 147, 136
	};

	unsigned char *in = temp;
	float *out = malloc(sizeof(float) * 64);
	unsigned char *inv = malloc(64);

	tdc(&out, &in);
	itdc(&inv, &out);

	// conferindo se o tdc e a inversa deu certo
	{
		int i;
		int diff;
		for(i = 0; i < 64; i++){
			if(in[i] != inv[i]){
				diff = in[i] - inv[i];
				// faixa de tolerancia de +/- 2 de diferenca
				assert(diff > -2 && diff < 2);
			}
		}
	}

	return 0;
}
