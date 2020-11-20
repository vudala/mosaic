// GRR20195689 Eduardo Vudala Senoski

#ifndef HEADER_DEFINED
#define HEADER_DEFINED

#include <stdio.h>

#define INVALID_TYPE -1
#define P3_TYPE 1
#define P6_TYPE 2
#define INVALID_TYPE_STR "!  INVALID  !"
#define P3_TYPE_STR "P3"
#define P6_TYPE_STR "P6"
#define HEADER_ATTRIBUTE_N 4 // Quantos atributos tem no header de um arquivo .ppm

struct header {
    int type; // O tipo da imagem
    int width; // A largura da imagem
    int height; // A altura da imagem
    int max_value; // O valor máximo dos pixeis da imagem
};

// Percorre um arquivo .ppm e retorna um struct header
struct header* getHeader(FILE* file);

// Constrói uma struct do tipo header e a retorna
struct header* newHeader(char* attr);

// Imprime o header em file
void printHeader(struct header* h, FILE* file);

#endif