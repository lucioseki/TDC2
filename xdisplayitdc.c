
// gcc xdisplayitdc.c tdc.c -o xdisplayitdc -lm -lX11
// ./xdisplayitdc lena512c.rawtdc

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
  Display               * display;
  XImage                * ximage;
  Window                window;
  GC                    gc;
  Visual                * visual;
  XGCValues             values;
  int                   m, n, FOREGROUNDP = 0, BACKGROUNDP = 1, screen, dplanes;
  int                   IMAGE_HEIGHT, IMAGE_WIDTH;
  int dim;

	float *redtdc;
	float *greentdc;
	float *bluetdc;

  fp = fopen("lena512c.rawtdc","rb");

	// recupera as TDCs do arquivo
	fread(&IMAGE_WIDTH, sizeof(int), 1, fp);
	fread(&IMAGE_HEIGHT, sizeof(int), 1, fp);

	dim = IMAGE_WIDTH * IMAGE_HEIGHT;
	redtdc = malloc(sizeof(float) * dim);
	greentdc = malloc(sizeof(float) * dim);
	bluetdc = malloc(sizeof(float) * dim);

	fread(redtdc, sizeof(unsigned char), dim, fp);
	fread(greentdc, sizeof(unsigned char), dim, fp);
	fread(bluetdc, sizeof(unsigned char), dim, fp);
  fclose(fp);

	unsigned char *reditdc = malloc(dim);
	unsigned char *greenitdc = malloc(dim);
	unsigned char *blueitdc = malloc(dim);

	// aplica a ITDC para cada banda
	itdc(&reditdc, &redtdc, dim);
	itdc(&greenitdc, &greentdc, dim);
	itdc(&blueitdc, &bluetdc, dim);

  if ((display = XOpenDisplay(NULL)) == NULL) {
     printf("Incapaz de conectar ao display...\n");
     exit(1);
     }
    
  screen = DefaultScreen(display);
  dplanes = DisplayPlanes(display,screen);   
  visual = XDefaultVisual(display,screen);
     
  FOREGROUNDP=BlackPixel(display,screen);
  BACKGROUNDP=WhitePixel(display,screen);
  
  if (!(window=XCreateSimpleWindow(display,RootWindow(display,screen),0,0,IMAGE_WIDTH,IMAGE_HEIGHT,1,FOREGROUNDP,BACKGROUNDP))) {
     printf("Não foi possível criar a janela...\n");
     exit(1);
     }

  XSelectInput(display, window, EventMask);
  gc = XCreateGC(display, window, 0, &values);
  
  XMapWindow(display,window);
  XSync(display,False);
  
  ximage = XCreateImage(display, visual, dplanes, ZPixmap, 0, malloc(IMAGE_WIDTH*IMAGE_HEIGHT*sizeof(int)), IMAGE_WIDTH, IMAGE_HEIGHT, 8, 0);

	// recupera da ITDC
  for(m=0;m<IMAGE_HEIGHT;m++) {
    for(n=0;n<IMAGE_WIDTH;n++) {
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4] = (char) blueitdc[m*IMAGE_WIDTH+n];
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+1] = (char) greenitdc[m*IMAGE_WIDTH+n];
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+2] = (char) reditdc[m*IMAGE_WIDTH+n];
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+3] = (char) 0;
      }
    }

  XPutImage(display, window, gc, ximage, 0, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

  sleep(10);
  
  return 0;
  }
