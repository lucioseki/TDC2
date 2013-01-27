/*
 * Le um arquivo RAW, aplica a TDC e salva num arquivo.
 *
 * compile com
 * gcc test_frame_tdc.c frame_tdc.c tdc.c -lm -o test_frame_tdc
 *
 * execute com
 * ./test_frame_tdc lena512c.raw 512 512
 *
 * */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame_tdc.h"

int main(int argc, char **argv){
	FILE *fp;
	int IMAGE_WIDTH, IMAGE_HEIGHT;
	int *intinput;
	Image *input;
	int i;

	// abre o arquivo
	fp = fopen(argv[1], "rb");

	IMAGE_WIDTH = atoi(argv[3]);
	IMAGE_HEIGHT = atoi(argv[2]);

	// le a imagem RAW. Cada pixel da lena512c.raw tem 12 bytes (4 bytes para cada cor).
	intinput = malloc(3 * sizeof(int) * IMAGE_WIDTH * IMAGE_HEIGHT);
	input = malloc(sizeof(Image) * IMAGE_WIDTH * IMAGE_HEIGHT);

	fread(intinput, sizeof(int), IMAGE_WIDTH * IMAGE_HEIGHT * 3, fp);
	fclose(fp);

	// converte para vetor de struct Image com 3 bytes
	for(i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; i++){
		input[i].red = intinput[i * 3];
		input[i].green = intinput[i * 3 + 1];
		input[i].blue = intinput[i * 3+ 2];
	}

	// prepara o vetor de TDC
	float *output = malloc(sizeof(float) * IMAGE_WIDTH * IMAGE_HEIGHT * 3);

	// aplica a TDC
	frame_tdc(&output, IMAGE_WIDTH, IMAGE_HEIGHT, &input);

	// salva o vetor de TDC
	fp = fopen(strcat(argv[1], "tdc"), "wb");
	fwrite(&IMAGE_WIDTH, sizeof(int), 1, fp);
	fwrite(&IMAGE_HEIGHT, sizeof(int), 1, fp);
	fwrite(output, sizeof(float), IMAGE_WIDTH * IMAGE_HEIGHT * 3, fp);
	fclose(fp);

	free(input);
	free(output);
	// free(intinput); // da segmentation fault quando dou free nele D=
	
	return 0;
}
