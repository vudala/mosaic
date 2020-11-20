// GRR20195689 Eduardo Vudala Senoski

#ifndef IMAGE_INCLUDED
#define IMAGE_INCLUDED

#include "header.h"
#include <stdio.h>

struct pixel;
struct header;

struct image {
    struct header* head; // O cabeçalho do arquivo
    struct pixel** pixels; // Uma matriz utilizada para armazenar os pixeis
    struct pixel* main_color; // Cor predominante da imagem
};

// Percorre a main_img e substitui suas tiles por tiles que tenham uma cor dominante igual ou parecida
void buildMosaic(struct image* main_img, struct image** tiles, int* tiles_n);

/* Navega por um diretório e retorna uma array image**,
sendo cada elemento um ponteiro para uma struct image* de um arquivo .ppm do diretório  */
struct image** getTiles(char* dirpath, int* tiles_n);

// Retorna uma struct image representando o arquivo
struct image* buildImage(FILE* img_file);

// Retorna uma struct image populada
struct image* newImage(struct header* h, struct pixel** pxs);

// Escreve a imagem no arquivo passado como parametro
void printImage(struct image* img, FILE* file);

// Libera os espacos de memória ocupado por uma struct image
void freeImage(struct image* img);

#endif