#ifndef TDC_H
#define TDC_H

// aplica tdc 
void tdc(float **out, unsigned char **input, int size);

// aplica tdc 
void itdc(unsigned char **inv, float **input, int size);

// compara o vetor original e a inversa da tdc
void conf(unsigned char **in, unsigned char **inv, int size);

#endif
