// GRR20195689 Eduardo Vudala Senoski

#include "pixel.h"
#include "utils.h"
#include "image.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct pixel* newPixel(int red, int green, int blue){
    struct pixel* p = malloc(sizeof(struct pixel));

    p->red = red;
    p->green = green;
    p->blue = blue;

    return p;
}

float redMean(struct pixel* px1, struct pixel* px2){
    float r = (px1->red + px2->red) / 2; // Red mean
    float d_red = removeSign(px1->red - px2->red); // Delta Red
    float d_green = removeSign(px1->green - px2->green); // Delta green
    float d_blue =  removeSign(px1->blue - px2->blue); // Delta blue

    return  sqrt(
                (2 + r / 256) * (powf(d_red, 2.0)) +
                (4 * powf(d_green, 2.0)) + 
                ((2 + (255 - r) / 256) * powf(d_blue, 2.0))
            );
}

struct pixel* getMainColor(struct image* img){
    long int red = 0, green = 0, blue = 0;

    // Soma todos os valores RGB ^ 2 para fazer a média entre eles posteriormente
    for(long int i = 0; i < img->head->height; i++)
        for(long int j = 0; j < img->head->width; j++){
            red = (red + img->pixels[i][j].red) ^ 2;
            green =  (green + img->pixels[i][j].green) ^ 2;
            blue = (blue + img->pixels[i][j].blue) ^ 2;
        }

    long int pixel_n = img->head->width * img->head->height; // Número de pixeis na imagem

    int red_m = (red / pixel_n) ^ (1/2); // Red main value
    int green_m = (green / pixel_n) ^ (1/2); // Green main value
    int blue_m = (blue / pixel_n) ^ (1/2); // Blue main value

    return newPixel(red_m, green_m, blue_m);
}

// Popula a matriz de pixeis com valores de um arquivo formato P6
struct pixel** populatePixelsP6(FILE* file, int width, int height){
    struct pixel** pxs = (struct pixel**) allocateMatrixMemory(sizeof(struct pixel), width, height);

    // Percorre o arquivo e armazena os bytes lidos em pxs
    fread(pxs[0], sizeof(struct pixel), width * height, file); 
    
    return pxs;
}

// Popula a matriz de pixeis com valores de um arquivo formato P3
struct pixel** populatePixelsP3(FILE* file, long int width, long int height){
    struct pixel** pxs = (struct pixel**) allocateMatrixMemory(sizeof(struct pixel), width, height);
    char* aux = NULL;
    char* tok = NULL;
    unsigned char* data = malloc(width * height * sizeof(struct pixel));
    int i = 0;
    while(fscanf(file, "%m[^\n]", &aux) > 0){ // Enquanto tiver conteudo pra ler, armazena o conteudo e o manipula
        fgetc(file); // Descarta o \n da stream
        eraseComment(aux);
        tok = strtok(aux, " ");
        while(tok != NULL){ // Enquanto houver conteúdo a ser lido da string
            data[i] = (unsigned char) atoi(tok); // Lê e armazena em bytes
            i++;
            tok = strtok(NULL, " ");
        }
        free(aux);
    }

    aux = NULL;
    // Copia os bytes lidos em data para pxs
    memcpy(pxs[0], data, width * height * sizeof(struct pixel));

    free(data);
    data = NULL;

    return pxs;
}

struct pixel** populatePixels(FILE* file, struct header* h){
    if(h->type == P6_TYPE)
        return populatePixelsP6(file, h->width, h->height);
    else if(h->type == P3_TYPE)
        return populatePixelsP3(file, h->width, h->height);

    return NULL;
}

void printPixel(struct pixel* p, FILE* file, int type){
    if(type == P3_TYPE)
        fprintf(file, "%i %i %i\n", p->red, p->green, p->blue);
    else if(type == P6_TYPE)
        fprintf(file, "%c%c%c", p->red, p->green, p->blue);
}