// GRR20195689 Eduardo Vudala Senoski

#include "image.h"
#include "pixel.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Sobrescreve uma parte da imagem
void replaceTile(struct image* main_img, int lin, int col, struct image* tile){
    int k, p;
    for(k = 0; k < tile->head->height; k++)
        for(p = 0; p < tile->head->width; p++)
            // Este if protege o programa de tentar escrever fora dos limites de memória
            if( (lin + k < main_img->head->height) && (col + p < main_img->head->width) )
                main_img->pixels[lin + k][col + p] = tile->pixels[k][p];
            
}

/* Retorna uma imagem que é um subespaço de main_img; lin e col determinam aonde deve começar o recorte,
width e height determinam o tamanho do recorte */
struct image* cropImg(struct image* main_img, int lin, int col, int width, int height){
    struct pixel** pxs = (struct pixel**) allocateMatrixMemory(sizeof(struct pixel), width, height);
    struct pixel* newPx = NULL;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++){
            // Este if protege o programa de tentar ler fora dos limites de memória
            if( (lin + i < main_img->head->height) && (col + j < main_img->head->width) )
                pxs[i][j] = main_img->pixels[i + lin][j + col];
            else {
                // Caso contrário, recebe um pixel branco para nao inteferir no valor do calculo de main_color
                newPx = newPixel(255, 255, 255);
                pxs[i][j] = *newPx;
                free(newPx);
            }
        }
    // Cria um header só com os atributos necessários para utilizar a funcao getMainColor
    struct header* h = malloc(sizeof(struct header));
    h->width = width;
    h->height = height;

    return newImage(h, pxs);
}

// Retorna uma struct image* que tenha ->main_color mais parecido com a main_color do parametro
struct image* similarTile(struct pixel* main_color, struct image** tiles, int* tiles_n){
    int i, pos = 0; // pos é o indice da tile com a cor mais próxima
    // Cor mais proxima
    float closest_c = redMean(main_color, tiles[pos]->main_color); // Atribui um valor inicial
    float current_c; // Cor atual
    for(i = 0; i < *tiles_n; i++){
        current_c = redMean(main_color, tiles[i]->main_color);
        if(closest_c > current_c){
            closest_c = current_c;
            pos = i;
        }
    }

    return tiles[pos];
}

void buildMosaic(struct image* main_img, struct image** tiles, int* tiles_n){
    if(*tiles_n < 1){
        fprintf(stderr, "No tiles were found to be used in the mosaic.\n");
        exit(0);
    }
    int tile_height = tiles[0]->head->height;
    int tile_width = tiles[0]->head->width;
    int i, j;
    struct image* tile = NULL;
    struct image* croppedImg = NULL;
    for(i = 0; i < main_img->head->height; i+= tile_height)
        for(j = 0; j < main_img->head->width; j+= tile_width){
            croppedImg = cropImg(main_img, i, j, tile_width, tile_height);
            tile = similarTile(croppedImg->main_color, tiles, tiles_n);
            replaceTile(main_img, i, j, tile);
            freeImage(croppedImg);
        }

    croppedImg = NULL;
    tile = NULL;
}

struct image** getTiles(char* dirpath, int* tiles_n){
    /* Percorre dirpath, armazena as entries com o final de direntry.d_name == .ppm
     e retorna um array com as struct image daqueles arquivos */
    struct dirent** namelist;
    int files_n = scandir(dirpath, &namelist, isPPM, NULL);
    if (files_n < 0) return NULL;
    *tiles_n = files_n; // Grava em tiles_n quantos arquivos foram lidos

    struct image** tiles = malloc(files_n * sizeof(struct image));
    FILE* img_file = NULL;
    char* filepath = malloc(sizeof(char) * MAXWORDSIZE);

    for(int i = 0; i < files_n; i++){
        // Monta o caminho completo do arquivo, o abre e o transforma em um struct image*
        strcpy(filepath, dirpath);
        formatFilePath(filepath);
        strcat(filepath, namelist[i]->d_name);
        img_file = fopen(filepath, "r");
        if(img_file == NULL) fireFileException(filepath);
        tiles[i] = buildImage(img_file);
        fclose(img_file);
    }

    // Limpa a memória reservada para as variáveis da função e as desaponta
    free(filepath);
    filepath = NULL;
    for(int i = 0; i < files_n; i++) free(namelist[i]);
    free(namelist);
    namelist = NULL;

    return tiles;
}

struct image* buildImage(FILE* img_file){
    struct header* h = getHeader(img_file);

    struct pixel** pxs = populatePixels(img_file, h);

    return newImage(h, pxs);
}

struct image* newImage(struct header* h, struct pixel** pxs){
    struct image* img = malloc(sizeof(struct image));

    img->head = h;
    img->pixels = pxs;
    img->main_color = getMainColor(img);

    return img;
}

void printImage(struct image* img, FILE* file){
    printHeader(img->head, file);
    for(int i = 0; i < img->head->height; i++)
        for(int j = 0; j < img->head->width; j++)
            printPixel(&(img->pixels[i][j]), file, img->head->type);
}

void freeImage(struct image* img){
    free(img->head);
    free(img->pixels[0]);
    free(img->pixels);
    free(img->main_color);
    free(img);
}