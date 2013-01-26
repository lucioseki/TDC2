TDC2
====

## Transformada do Cosseno sobre uma imagem ##

É um programa para fins didáticos que realiza a transformada discreta do cosseno (TDC) sobre um vetor imagem.

## Descrição dos Arquivos ##
- tdc.c implementa as funções blocktdc e blockitdc, que realiza a transformada e a sua inversa, respectivamente, em blocos de 8 x 8.

- tdc.h lista as funções tdc e itdc do tdc.c. Estas funções aplicam a TDC e ITDC para arquivos com dimensões múltiplas de 8 x 8.

- test.c realiza testa estas funções com um vetor de 576 posições, representando uma imagem 24 x 24.

- xdisplayrawtdc.c lê uma imagem RAW, aplica a TDC em cada banda de cor, recupera os valores originais com a ITDC e mostra a imagem resultante.
