// GRR20195689 Eduardo Vudala Senoski

#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <stdio.h>
#include <dirent.h>

#define MAXWORDSIZE 1024
#define DEFAULT_DELIMITER " "
#define PPM_FILEFORMAT ".ppm"
#define COMMENT_CHAR '#' // Char utilizado para comentar um arquivo .ppm

// Verifica se o arquivo é .ppm
int isPPM(const struct dirent* direntry);

// Apaga o comentário de uma string
void eraseComment(char* str);

// Aloca um espaço de memória para uma matriz com elementos de size bytes
void** allocateMatrixMemory(int size, long int width, long int height);

// Dispara um erro de memória com o nome da funcao onde foi executada 
void fireMemException(const char* caller);

// Dispara um erro informando que falhou em abrir o arquivo
void fireFileException(char* filename);

// Converte um N negativo em positivo
float removeSign(float n);

// Concatena as strings, as separando por um DEFAULT_DELIMITER
void appendString(char* target, char* str);

#endif