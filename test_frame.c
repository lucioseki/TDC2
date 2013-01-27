/*
 *	compile com
 *	gcc test_save.c frame_tdc.c tdc.c -lm -o test_save
 *
 *	execute com
 *	./test_save lena512c.raw 512 512
 *
 * */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame_tdc.h"

int main(int argc, char **argv){
	
	// abre o arquivo
	FILE *fp = fopen(argv[1], "rb");
	assert(fp);

	int IMAGE_WIDTH = atoi(argv[3]);
	int IMAGE_HEIGHT = atoi(argv[2]);

	// le a imagem RAW
	Image *input = malloc(sizeof(Image) * IMAGE_WIDTH * IMAGE_HEIGHT);
	fread(input, sizeof(Image), IMAGE_WIDTH * IMAGE_HEIGHT, fp);
	fclose(fp);

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

	return 0;
}
