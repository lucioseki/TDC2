#include <math.h>
// metade da raiz de 2
#define r2_2 0.707106781

float C(char c){
	return c == 0? r2_2 : 1;
}

// recebe ponteiro para vetor in de bytes
// guarda os valores da tdc no vetor out de floats
void tdc(float **out, unsigned char **in){
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
void itdc(unsigned char **out, float **in){
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
