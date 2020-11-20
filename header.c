// GRR20195689 Eduardo Vudala Senoski

#include "header.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

struct header* getHeader(FILE* file){
    char* attr = calloc(sizeof(char), MAXWORDSIZE); // String de atributos
    char* aux = NULL; // String a ser tokenizada
    char* token = NULL; // String para armazenar os tokens 
    
    int i = 0;
    /* Enquanto não preencheu N atributos */
    while(i <  HEADER_ATTRIBUTE_N){
        /* Captura a linha da stream, remove o \n, remove comentários, 
        tokeniza a string e caso o token seja um argumento válido o armazena em attr */
        fscanf(file, "%m[^\n]", &aux);
        fgetc(file); // Descarta o \n da stream
        eraseComment(aux);
        token = strtok(aux, " ");
        while(token != NULL){
            appendString(attr, token); // Adiciona um novo atributo na string
            i++;
            token = strtok(NULL, " ");
        }
        free(aux);
    }
    aux = NULL;

    return newHeader(attr);
}

// Converte o PPM type de string para uma constante int definida
const int strConvertType(char* type){
    if( !strcmp((type), P3_TYPE_STR) ) return P3_TYPE;
    if( !strcmp((type), P6_TYPE_STR) ) return P6_TYPE;

    return INVALID_TYPE;
}

// Converte o PPM type de string para uma constante int definida
const char* intConvertType(int type){
    if( type == P3_TYPE ) return P3_TYPE_STR;
    if( type == P6_TYPE ) return P6_TYPE_STR;
        
    return INVALID_TYPE_STR;
}

struct header* newHeader(char* attr){
    struct header* h = malloc(sizeof(struct header));

    // Recupera cada atributo, baseado num DEFAULT_DELIMITER
    h->type = strConvertType(strtok(attr, DEFAULT_DELIMITER));
    if (h->type == INVALID_TYPE) return NULL;
    h->width = atoi(strtok(NULL, DEFAULT_DELIMITER));
    h->height = atoi(strtok(NULL, DEFAULT_DELIMITER));
    h->max_value = atoi(strtok(NULL, DEFAULT_DELIMITER));

    free(attr);

    return h;
}

void printHeader(struct header* h, FILE* file){
    fprintf(file, "%s\n%i %i\n%i\n", intConvertType(h->type), h->width, h->height, h->max_value);
}