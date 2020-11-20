// GRR20195689 Eduardo Vudala Senoski

#include "utils.h"
#include <stdlib.h>
#include <string.h>

void eraseComment(char* str){
    if(str == NULL) return;
    for(int i = 0; i < strlen(str); i++)
        if(str[i] == COMMENT_CHAR){
            str[i] = '\0';
            return;
        }
}

int isPPM(const struct dirent * direntry){
    const char* filename = (char*) direntry->d_name;
    const int type = direntry->d_type;
    int format_size = strlen(PPM_FILEFORMAT);
    int r = 0; // Valor de retorno
    // Usa format_size e copia a mesma quantidade de chars do fim de filename
    char* format = malloc(MAXWORDSIZE);
    strcpy(format, &filename[strlen(filename) - format_size]);
    if( !strcmp(format, PPM_FILEFORMAT) && type == DT_REG) r = 1;

    free(format);
    format = NULL;
    
    return r;
}

void appendString(char* target, char* str){
    int len = strlen(target);
    if(len == 0){
        strcpy(target, str);
        return;
    }
    strcat(target, DEFAULT_DELIMITER);
    strcat(target, str);
}

float removeSign(float n){
    return (n < 0) ? (n * -1) : n;
}

void** allocateMatrixMemory(int size, long int width, long int height){
    void** m;

    m = malloc(height * sizeof(void*));
    m[0] = malloc(height * width * size);
    for (int i=1; i < height; i++)
        m[i] = m[0] + i * width * size;

    return m;
}

void fireMemException(const char* caller){
    fprintf(stderr, "Error while trying to allocate memory: %s().\n", caller);
    exit(1);
}

void fireFileException(char* filename){
    fprintf(stderr, "Error while trying to open file: %s.\n", filename);
    exit(1);
}