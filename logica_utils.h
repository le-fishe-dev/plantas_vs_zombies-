#ifndef LOGICA_UTILS_H
#define LOGICA_UTILS_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entidades.h"
#include "proyectiles.h"


/*
Función que devuelve el índice de un elemento dentro de su arreglo, ya sea proyectil,
entidad, etc.
*/
int encuentra_indice_arreglo(int filas, int columnas, int ***Matriz, int fila_elemento, int columna_elemento);

/*
Función que genera un arreglo de 'proyectiles', correspondientes a estructuras
definidas en 'proyectiles.h'. El arreglo usa memoria dinámica mediante el uso
de la función malloc(). 
*/
int genera_arreglo_proyectiles(proyectil **array, int cantidad_proyectiles);

/*
Función que genera un arreglo de 'entidades', las cuales corresponden
a estructuras definidas en 'entidades.h'. El arreglo usa memoria dinámica
mediante la función malloc().
*/
int genera_arreglo_entidades(entidad **array, int cantidad_entidades);

/*
Función que devuelve la cantidad de tipos distintos de zombies que hay
almacenados en el archivo 'entidades.txt'.
*/
int obtiene_cantidad_tipos_zombies();

/*
Función que llena una matriz con enteros mediante scanf().
*/
void llena_matriz(int ***M, int n, int m);

/*
Función que genera una matriz n x m de memoría dinámica con malloc().
*/
void genera_matriz(int ***M, int n, int m);

/*
Función que busca en el archivo 'proyectiles.txt' por el valor del 
atributo asociado al tipo de proyectil especificado. Hace un uso
considerable de funciones asociadas al manejo de archivos.
*/
int obtiene_proyectil(char atributo[], int tipo_entidad);

/*
Función que busca en el archivo 'entidades.txt' por el valor del 
atributo asociado al tipo de entidad especificada. Hace un uso
considerable de funciones asociadas al manejo de archivos.
*/
int obtiene_entidad(char atributo[], int tipo_entidad);

#endif