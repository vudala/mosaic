// GRR20195689 Eduardo Vudala Senoski

#include "image.h"
#include "utils.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_TILES_PATH "./tiles/"

int main(int argc, char** argv){
    FILE* input_file = stdin;
    FILE* output_file = stdout;

    char* tiles_path = malloc(sizeof(char) * MAXWORDSIZE);
    strcpy(tiles_path, DEFAULT_TILES_PATH);

    char opt;
    opterr = 0; // Impede que o getopt imprima a mensagem padrão de erro
    while((opt = getopt(argc, argv, "i:o:p:")) != -1)
        switch(opt){
            case 'i':
                input_file = fopen(optarg, "r");
                if(input_file == NULL) fireFileException(optarg);
                break;
            case 'o':
                output_file = fopen(optarg, "w");
                if(output_file == NULL) fireFileException(optarg);
                break;
            case 'p':
                strcpy(tiles_path, optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -i [input.ppm] -p [pathtotiles] -o [output]\n", argv[0]);
                break;
        }

    int tiles_n = 0;
    struct image** tiles = getTiles(tiles_path, &tiles_n);
    if(tiles == NULL) fireFileException(tiles_path);
    free(tiles_path);
    tiles_path = NULL;

    struct image* main_img = buildImage(input_file);
    buildMosaic(main_img, tiles, &tiles_n);
    printImage(main_img, output_file);

    // Limpa e desaponta todas as estruturas criadas para gerar o mosaico
    freeImage(main_img);
    main_img = NULL;
    for(int i = 0; i < tiles_n; i++) freeImage(tiles[i]);
    free(tiles);
    tiles = NULL;
    fclose(input_file);
    input_file = NULL;
    fclose(output_file);
    output_file = NULL;

    return 0;
}   