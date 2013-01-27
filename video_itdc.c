/*
* Aplicação da ITDC sobre frames de um video
*
* compile com
* gcc video_itdc.c frame_tdc.c tdc.c -lm -lSDL -o video_itdc
*
* execute com
*./video_itdc sample_video.rawtdc
*
*/

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "frame_tdc.h"

int main(int argc, char **argv)
{   
  SDL_Surface *screen;
  FILE *fp;
  char *buf;
  int n, i;
	int VIDEO_WIDTH, VIDEO_HEIGHT;
	Image *frame;
	float *input;

  //hard-code file name and parameters just for demo purposes
  if ( ( fp = fopen ( "sample_video.rawtdc", "rb" ) ) == NULL ) {
    printf("\nError opening file \n");
    exit ( 0 );
  }
	fread(&VIDEO_WIDTH, sizeof(int), 1, fp);
	fread(&VIDEO_HEIGHT, sizeof(int), 1, fp);

  // buffer para armazenar um frame
	frame = malloc(sizeof(Image) * VIDEO_WIDTH * VIDEO_HEIGHT);
	input = malloc(sizeof(float) * 3 * VIDEO_WIDTH * VIDEO_HEIGHT);
	
	/*-------------------- Funções da SDL ---------------------------*/

  //initialize video system
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
        exit(1);
  }
  //ensure SDL_Quit is called when the program exits
  atexit(SDL_Quit);
   
  //set video mode of 320 x 240 with 24-bit pixels
  screen = SDL_SetVideoMode( VIDEO_WIDTH, VIDEO_HEIGHT, 24, SDL_SWSURFACE);
  if ( screen == NULL ) {
        fprintf(stderr, "Unable to set %dx%d video: %s\n", VIDEO_WIDTH, VIDEO_HEIGHT, SDL_GetError());
        exit(1);
  }
	/*--------------------------------------------------------------*/

  screen->pixels = frame;	//point framebuffer to data buffer
  while ( 1 ) {
    // read data ( one frame of n bytes ) into buffer
    if ( fread ( input, sizeof(float), 3 * VIDEO_WIDTH * VIDEO_HEIGHT, fp )  <= 0 )
	break;
		
		frame_itdc(&frame, VIDEO_WIDTH, VIDEO_HEIGHT, &input);
    SDL_Delay ( 50 );				//50 ms per frame
    SDL_UpdateRect ( screen, 0, 0, 0, 0 );	//blit data to screen
  }  
  fclose ( fp );
  SDL_Delay ( 2000 );	//delay 2 seconds before exit
  printf("Playing video successful!\n");
  return 0;
}
 
