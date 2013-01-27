/*
 * Le um arquivo com TDC, aplica a ITDC e exibe na tela.
 *
 * compile com:
 * gcc test_frame_itdc.c frame_tdc.c tdc.c -o test_frame_itdc -lm -lX11
 *
 * execute com:
 * ./test_frame_itdc lena512c.rawtdc
*/

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
	float *tdc;

  fp = fopen(argv[1],"rb");

	// recupera a tdc do arquivo
	fread(&IMAGE_WIDTH, sizeof(int), 1, fp);
	fread(&IMAGE_HEIGHT, sizeof(int), 1, fp);

	tdc = malloc(sizeof(float) * 3 * IMAGE_WIDTH * IMAGE_HEIGHT);
	fread(tdc, sizeof(float), 3 * IMAGE_WIDTH * IMAGE_HEIGHT, fp);
  fclose(fp);

	// aplica a itdc 
	Image *itdc = malloc(sizeof(Image) * IMAGE_WIDTH * IMAGE_HEIGHT);
	frame_itdc(&itdc, IMAGE_WIDTH, IMAGE_HEIGHT, &tdc);

  if ((display = XOpenDisplay(NULL)) == NULL) {
     printf("Incapaz de conectar ao display...\n");
     exit(1);
  }
    
	/* ---------------- funcoes do XDisplay ---------------- */
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
	/* ------------------------------------------------ */

	// Carrega os valores da itdc para exibir 
  for(m=0;m<IMAGE_HEIGHT;m++) {
    for(n=0;n<IMAGE_WIDTH;n++) {
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4] = itdc[m*IMAGE_WIDTH+n].blue;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+1] = itdc[m*IMAGE_WIDTH+n].green;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+2] = itdc[m*IMAGE_WIDTH+n].red;
      ximage -> data[(m*4)*IMAGE_WIDTH+n*4+3] = 0;
      }
    }

  XPutImage(display, window, gc, ximage, 0, 0, 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

  sleep(10);
  
	free(tdc);
	free(itdc);

  return 0;
}
