/*
* Aplicação da TDC sobre frames de um video
*
* compile com
* gcc video_tdc.c frame_tdc.c tdc.c -lm -o video_tdc
*
* execute com
*./video_tdc sample_video.raw 320 240
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "frame_tdc.h"

int main(int argc, char **argv)
{   
  FILE *infile, *outfile;
	Image *frame;
	int VIDEO_WIDTH, VIDEO_HEIGHT;
	float *output;
	int nframe;

  //hard-code file name and parameters just for demo purposes
  if ( ( infile = fopen ( argv[1], "rb" ) ) == NULL ) {
    printf("\nError opening input file \n");
    exit ( 0 );
  }

	VIDEO_WIDTH = atoi(argv[2]);
	VIDEO_HEIGHT = atoi(argv[3]);

	if ( ( outfile = fopen( strcat(argv[1], "tdc"), "wb" ) ) == NULL) {
		printf("\nError opening output file \n");
		exit ( 0 );
	}

	// salva as dimensoes do video
	fwrite(&VIDEO_WIDTH, sizeof(int), 1, outfile);
	fwrite(&VIDEO_HEIGHT, sizeof(int), 1, outfile);

  // buffer para armazenar um frame
	frame = malloc(sizeof(Image) * VIDEO_WIDTH * VIDEO_HEIGHT);
	output = malloc(sizeof(float) * 3 * VIDEO_WIDTH * VIDEO_HEIGHT);
	
	nframe = 0;
  while ( 1 ) {
		// le um frame
    if ( fread ( frame, sizeof(Image), VIDEO_WIDTH * VIDEO_HEIGHT, infile)  <= 0 )
			break;
		
		// mostra o frame atual, elemento de visibilidade do estado.
		printf("\rframe %d", ++nframe);
		fflush(stdout);

		// aplica a TDC
		frame_tdc(&output, VIDEO_WIDTH, VIDEO_HEIGHT, &frame);

		// salva a TDC no arquivo
		fwrite(output, sizeof(float), 3 * VIDEO_WIDTH * VIDEO_HEIGHT, outfile);
  }

	free(frame);
	free(output);
  fclose ( infile );
  fclose ( outfile );

  return 0;
}
