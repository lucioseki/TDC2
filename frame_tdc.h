#ifndef SAVE_TDC_H
#define SAVE_TDC_H

// estrutura para representar um pixel da imagem colorida
typedef struct{
	unsigned int red, green, blue;
}Image;

// recebe um vetor de pixels e as dimensões de uma imagem
// joga no vetor output as tdcs de cada banda na sequencia R, G, B
void frame_tdc(float **output, int IMAGE_WIDTH, int IMAGE_HEIGHT, Image **input);

#endif
