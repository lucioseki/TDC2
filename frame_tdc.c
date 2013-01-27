#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tdc.h"
#include "frame_tdc.h"

// recebe um vetor de pixels e as dimensões de uma imagem
// joga no vetor output as tdcs de cada banda na sequencia R, G, B
void frame_tdc(output, IMAGE_WIDTH, IMAGE_HEIGHT, input)
	float **output;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;
	Image **input;
{
  int m, n;
  
	// vetor para cada banda de cor
	unsigned char *reds = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *greens = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *blues = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);

	// ponteiro para indicar a posicao do vetor de saida
	float *position;

	// armazena cada banda num vetor
  for(m=0;m<IMAGE_HEIGHT;m++) {
    for(n=0;n<IMAGE_WIDTH;n++) {
      reds[m*IMAGE_WIDTH + n] = (unsigned char) (*input)[m*IMAGE_WIDTH+n].red;
      greens[m*IMAGE_WIDTH + n] = (unsigned char) (*input)[m*IMAGE_WIDTH+n].green;
      blues[m*IMAGE_WIDTH + n] = (unsigned char) (*input)[m*IMAGE_WIDTH+n].blue;
		}
	}

	// aplica a TDC para cada banda e armazena no vetor output
	// posicao da banda vermelha
	position = *output;
	tdc(&position, &reds, IMAGE_WIDTH * IMAGE_HEIGHT);

	// avanca para a posicao da banda verde
	position += IMAGE_WIDTH * IMAGE_HEIGHT;
	tdc(&position, &greens, IMAGE_WIDTH * IMAGE_HEIGHT);

	// avanca para a posicao da banda azul
	position += IMAGE_WIDTH * IMAGE_HEIGHT;
	tdc(&position, &blues, IMAGE_WIDTH * IMAGE_HEIGHT);

	free(reds);
	free(greens);
	free(blues);
}

// recebe um vetor das tdcs de cada banda na sequencia R, G, B
// salva a itdc no vetor de pixels
void frame_itdc(output, IMAGE_WIDTH, IMAGE_HEIGHT, input)
	Image ** output;
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;
	float **input;
{
	unsigned char *reditdc = malloc(IMAGE_WIDTH * IMAGE_HEIGHT);
	unsigned char *greenitdc = malloc(IMAGE_WIDTH * IMAGE_HEIGHT);
	unsigned char *blueitdc = malloc(IMAGE_WIDTH * IMAGE_HEIGHT);
	float *p;
	int m, n;

	// aplica a ITDC para cada banda
	p = *input;
	itdc(&reditdc, &p, IMAGE_WIDTH * IMAGE_HEIGHT);

	p += IMAGE_WIDTH * IMAGE_HEIGHT;
	itdc(&greenitdc, &p, IMAGE_WIDTH * IMAGE_HEIGHT);

	p += IMAGE_WIDTH * IMAGE_HEIGHT;
	itdc(&blueitdc, &p, IMAGE_WIDTH * IMAGE_HEIGHT);
	
	// recupera da ITDC
	for(m=0;m<IMAGE_HEIGHT;m++) {
		for(n=0;n<IMAGE_WIDTH;n++) {
			(*output)[m*IMAGE_WIDTH+n].red = (unsigned char) reditdc[m*IMAGE_WIDTH+n];
			(*output)[m*IMAGE_WIDTH+n].green = (unsigned char) greenitdc[m*IMAGE_WIDTH+n];
			(*output)[m*IMAGE_WIDTH+n].blue = (unsigned char) blueitdc[m*IMAGE_WIDTH+n];
		}
	}

	free(reditdc);
	free(greenitdc);
	free(blueitdc);
}
