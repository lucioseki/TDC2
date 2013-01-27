/*
 * Compile com:
 * gcc test.c tdc.c -lm
 *
 * */

#include <malloc.h>
#include "tdc.h"

int main(){

	// matriz exemplo
	unsigned char input[] = {
		140, 144, 147, 140, 140, 155, 179, 175,
		144, 152, 140, 147, 140, 148, 167, 179,
		152, 155, 136, 167, 163, 162, 152, 172,
		168, 145, 156, 160, 152, 155, 136, 160,
		162, 148, 156, 148, 140, 136, 147, 162,
		147, 167, 140, 155, 155, 140, 136, 162,
		136, 156, 123, 167, 162, 144, 140, 147,
		148, 155, 136, 155, 152, 147, 147, 136
	};

	float *out = malloc(sizeof(float) * sizeof(input));
	unsigned char *inv = malloc(sizeof(input));
	unsigned char *in = input;

	tdc(&out, &in, sizeof(input));
	itdc(&inv, &out, sizeof(input));
	conf(&in, &inv, sizeof(input));

	return 0;
}
