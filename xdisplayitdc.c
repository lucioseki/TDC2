
// gcc xdisplayitdc.c tdc.c -o xdisplayitdc -lm -lX11
// ./xdisplayitdc lena512c.rawtdc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tdc.h"
#include "frame_tdc.h"

#include <X11/Xlib.h>

#define EventMask (ExposureMask)

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

	float *tdc;

  fp = fopen(argv[1],"rb");

	// recupera as TDCs do arquivo
	fread(&IMAGE_WIDTH, sizeof(int), 1, fp);
	fread(&IMAGE_HEIGHT, sizeof(int), 1, fp);

	dim = IMAGE_WIDTH * IMAGE_HEIGHT;
	tdc = malloc(sizeof(Image) * dim);

	fread(tdc, sizeof(Image), dim, fp);
  fclose(fp);

	Image *itdc = malloc(sizeof(Image) * dim);

	// aplica a ITDC para cada banda
	frame_itdc(&itdc, IMAGE_WIDTH, IMAGE_HEIGHT, &tdc);

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
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4] = (char) itdc[m*IMAGE_WIDTH+n].blue;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+1] = (char) itdc[m*IMAGE_WIDTH+n].green;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+2] = (char) itdc[m*IMAGE_WIDTH+n].red;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+3] = (char) 0;
      }
    }

  XPutImage(display, window, gc, ximage, 0, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

  sleep(10);
  
	free(tdc);
	free(itdc);
  return 0;
  }
