
// gcc save_tdc.c tdc.c -o savetdc -lm -lX11
// ./savetdc lena512c.raw 512 512

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tdc.h"

#include <X11/Xlib.h>

#define EventMask (ExposureMask)

struct Image {
     unsigned int red, green, blue;
     };
     
int main(int argc, char ** argv) {
  FILE                  * fp;
  struct Image          * image;
  int                   m, n;
  int                   IMAGE_HEIGHT, IMAGE_WIDTH;
  
  IMAGE_WIDTH = (int) atoi(argv[2]);
  IMAGE_HEIGHT = (int) atoi(argv[3]);

  if ((fp=fopen(argv[1],"rb"))==NULL) exit(1);
  else {
    image = (struct Image *) malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(struct Image));
    if (!image) exit(1);
    fread(image,IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(struct Image),1,fp);
    fclose(fp);
    }  

	unsigned char *reds = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *greens = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *blues = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);

	float *redtdc = malloc(sizeof(float) * IMAGE_WIDTH*IMAGE_HEIGHT);
	float *greentdc = malloc(sizeof(float) * IMAGE_WIDTH*IMAGE_HEIGHT);
	float *bluetdc = malloc(sizeof(float) * IMAGE_WIDTH*IMAGE_HEIGHT);

	// armazena cada banda num vetor
  for(m=0;m<IMAGE_HEIGHT;m++) {
    for(n=0;n<IMAGE_WIDTH;n++) {
      reds[m*IMAGE_WIDTH + n] = (unsigned char) image[m*IMAGE_WIDTH+n].red;
      greens[m*IMAGE_WIDTH + n] = (unsigned char) image[m*IMAGE_WIDTH+n].green;
      blues[m*IMAGE_WIDTH + n] = (unsigned char) image[m*IMAGE_WIDTH+n].blue;
		}
	}

	// aplica a TDC para cada banda
	tdc(&redtdc, &reds, IMAGE_WIDTH * IMAGE_HEIGHT);
	tdc(&greentdc, &greens, IMAGE_WIDTH * IMAGE_HEIGHT);
	tdc(&bluetdc, &blues, IMAGE_WIDTH * IMAGE_HEIGHT);

	// armazena as TDCs num arquivo
	fp = fopen(strcat(argv[1], "tdc"), "wb");
	fwrite(&IMAGE_WIDTH, sizeof(int), 1, fp);
	fwrite(&IMAGE_HEIGHT, sizeof(int), 1, fp);
	fwrite(redtdc, sizeof(float), IMAGE_WIDTH * IMAGE_HEIGHT, fp);
	fwrite(greentdc, sizeof(float), IMAGE_WIDTH * IMAGE_HEIGHT, fp);
	fwrite(bluetdc, sizeof(float), IMAGE_WIDTH * IMAGE_HEIGHT, fp);
	fclose(fp);

	free(reds);
	free(greens);
	free(blues);
	
	free(redtdc);
	free(greentdc);
	free(bluetdc);

	free(image);
	
  return 0;
}
