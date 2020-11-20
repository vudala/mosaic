#ifndef PIXEL_INCLUDED
#define PIXEL_INCLUDED

#include <stdio.h>

struct image;
struct header;

#define RGB_ATTR_N 3 // Número de atributos em uma estrutura RGB

struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

// Retorna uma struct pixel populada, recebendo com argumento tres parametros int para popular a struct pixel
struct pixel* newPixel(int red, int green, int blue);

// Calcula a cor dominante da imagem, utilizando a raiz da média dos quadrados dos valores dos pixeis
struct pixel* getMainColor(struct image* img);

// Calcula a distância entre duas cores utilizando a fórmula Red Mean Value
float redMean(struct pixel* px1, struct pixel* px2);

// Decide qual será a função que deve ser ativada para popular os pixeis
struct pixel** populatePixels(FILE* file, struct header* h);

// Imprime um pixel no arquivo do parametro
void printPixel(struct pixel* p, FILE* file, int type);

#endif