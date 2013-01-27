/*
 * Implementacao das funcoes TDC e ITDC.
 *
 * */
#ifndef TDC_H
#define TDC_H

#include <stdlib.h>
#include <assert.h>
#include <math.h>

// metade da raiz de 2
#define r2_2 0.707106781

float C(char c){
	return c == 0? r2_2 : 1;
}

// recebe ponteiro para vetor in de bytes
// guarda os valores da tdc no vetor out de floats
void blocktdc(float **out, unsigned char **in){
	int i, j, u, v;
	float sum;

	// janela de 8 x 8
	for(u=0;u<8;u++) {
	  for(v=0;v<8;v++) {
			sum = 0;
			for(i = 0; i < 8; i++){
				for(j = 0; j < 8; j++){
					sum += cos( (( 2*i+1 )*u*M_PI) / 16 ) * cos( (( 2*j+1 )*v*M_PI) / 16 ) * (*in)[i*8 + j];
				}
			}

			(*out)[u*8 + v] = ((C(u) * C(v))/4) * sum;
     }
   }
}

// recebe ponteiro para vetor in de floats
// guarda os valores da inversa no vetor out de bytes 
void blockitdc(unsigned char **out, float **in){
	int i, j, u, v;
	float sum;

	// janela de 8 x 8
	for(i=0;i<8;i++) {
	  for(j=0;j<8;j++) {
			sum = 0;
			for(u = 0; u < 8; u++){
				for(v = 0; v < 8; v++){
					sum += ((C(u) * C(v))/4) * cos( (( 2*i+1 )*u*M_PI) / 16 ) * cos( (( 2*j+1 )*v*M_PI) / 16 ) * (*in)[u*8 + v];
				}
			}
			(*out)[i*8 + j] = sum;
     }
   }
}

// aplica tdc 
void tdc(float **out, unsigned char **input, int size){
	int i, j, k, l;

	// quantidade de blocos 8 x 8
	int numblocks = size / 64;

	// quantidade de blocos na vertical e na horizontal
	int vblocks = pow(numblocks, 0.5);
	int hblocks = pow(numblocks, 0.5);

	// vetores temporarios para blocos 8 x 8
	unsigned char *tempin = malloc(64);
	float *tempout = malloc(sizeof(float)*64);

	// para cada bloco
	for(i = 0; i < vblocks; i++){
		for(j = 0; j < hblocks; j++){

			// copia o bloco da entrada
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					tempin[k * 8 + l] = (*input)[ i*(size / vblocks) + j*8 + k*8*hblocks + l ];
				}
			}

			// aplica a tdc sobre o bloco
			blocktdc(&tempout, &tempin);

			// copia o bloco para a saida da tdc
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					(*out)[ i*(size / vblocks) + j*8 + k*8*hblocks + l ] = tempout[k * 8 + l];
				}
			}

		} // proximo hblock
	} // proximo vblock 
} // fim tdc

// aplica tdc 
void itdc(unsigned char **inv, float **input, int size){
	int i, j, k, l;

	// quantidade de blocos 8 x 8
	int numblocks = size / 64;

	// quantidade de blocos na vertical e na horizontal
	int vblocks = pow(numblocks, 0.5);
	int hblocks = pow(numblocks, 0.5);

	// vetores temporarios para blocos 8 x 8
	float *tempin = malloc(sizeof(float) * 64);
	unsigned char *tempout = malloc(64);

	// para cada bloco
	for(i = 0; i < vblocks; i++){
		for(j = 0; j < hblocks; j++){

			// copia o bloco da entrada
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					tempin[k * 8 + l] = (*input)[ i*(size / vblocks) + j*8 + k*8*hblocks + l ];
				}
			}

			// aplica a inversa da tdc sobre o bloco
			blockitdc(&tempout, &tempin);

			// copia o bloco para a saida da itdc
			for(k = 0; k < 8; k++){
				for(l = 0; l < 8; l++){
					(*inv)[ i*(size / vblocks) + j*8 + k*8*hblocks + l ] = tempout[k * 8 + l];
				}
			}

		} // proximo hblock
	} // proximo vblock 

} // fim itdc

// compara o vetor original e a inversa da tdc
void conf(unsigned char **in, unsigned char **inv, int size) {
	int i;
	int diff;
	for(i = 0; i < size; i++){
		if((*in)[i] != (*inv)[i]){
			diff = (*in)[i] - (*inv)[i];
			// faixa de tolerancia de +/- 2 de diferenca
			assert(diff > -2 && diff < 2);
		}
	}
}

#endif
