TDC2
====

## Transformada do Cosseno sobre uma imagem ##

É um programa para fins didáticos que realiza a transformada discreta do cosseno (TDC) sobre um vetor imagem.

## Descrição dos Arquivos ##
- tdc.c implementa as funções blocktdc e blockitdc, que realiza a transformada e a sua inversa, respectivamente, em blocos de 8 x 8. Implementa também as funções tdc e itdc, que realiza as operações para cada bloco de um vetor maior.

- tdc.h lista as funções blocktdc, blockitdc, tdc e itdc do tdc.c.

- frame\_tdc.c implementa as funções frame\_tdc e frame\_itdc, que chama as funções tdc e itdc num nível de abstração mais alto, trabalhando com vetor de uma estrutura que representa um pixel colorido.

- frame\_tdc.h descreve a estrutura Image e lista as funções frame\_tdc e frame\_itdc.

- test\_tdc\_itdc.c testa as funções acima com um vetor de 64 posições, representando uma imagem 8 x 8.

- test\_frame\_tdc.c lê uma imagem RAW, aplica a TDC em cada banda de cor e salva o resultado num arquivo.

- test\_frame\_itdc.c lê o arquivo gerado pelo test\_frame\_tdc.c, aplica a ITDC para recuperar os valores originais e mostra a imagem resultante.
