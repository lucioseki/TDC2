
// gcc xdisplayraw.c tdc.c -o xdisplayraw -lX11
// ./xdisplayraw lena512c.raw 512 512

#include <stdio.h>
#include <stdlib.h>
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
  Display               * display;
  XImage                * ximage;
  Window                window;
  XEvent                an_event;
  GC                    gc;
  Visual                * visual;
  XGCValues             values;
  int                   m, n, FOREGROUNDP = 0, BACKGROUNDP = 1, screen, dplanes;
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

	unsigned char *reditdc = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *greenitdc = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);
	unsigned char *blueitdc = malloc(IMAGE_WIDTH*IMAGE_HEIGHT);

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
	
	// recupera as TDCs do arquivo

	// aplica a ITDC para cada banda
	itdc(&reditdc, &redtdc, IMAGE_WIDTH * IMAGE_HEIGHT);
	itdc(&greenitdc, &greentdc, IMAGE_WIDTH * IMAGE_HEIGHT);
	itdc(&blueitdc, &bluetdc, IMAGE_WIDTH * IMAGE_HEIGHT);

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
