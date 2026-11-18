#ifndef PROYECTILES_H
#define PROYECTILES_H
#include <stdio.h>
#include <stdlib.h>

/*
Estructura que almacena los datos de cada proyectil del juego.
*/
typedef struct Proyectil
{
	int ataque;								// daño que genera su impacto

	float tiempo_en_recorrer_casilla;		// segundos en atravesar una casilla
	float contador_para_recorrer_casilla;

	int esta_congelado;						// modificador booleano, ralentiza a los zombies
	int esta_ardiendo;						// modificador booleano, aumenta el daño

} proyectil;

/*
Función que agrega un nuevo proyectil en el arreglo de proyectiles,
siendo especificado el índice donde se desea agregar el nuevo proyectil 
y el tipo de proyectil a agregar.
*/
void agrega_proyectil(int indice, proyectil **array, int *cantidad_proyectiles, int tipo_proyectil);

/*
Función que elimina un proyectil del arreglo de proyectiles, siendo
específicado el índice del proyectil que se desea eliminar.
*/
void elimina_proyectil(int indice, proyectil **array, int *cantidad_proyectiles);

/*
Función que directamente define los valores enteros de cada atributo
de un proyectil, obteniendo aquel valor entero desde la función 'obtiene()',
definida en el header 'logica_utils.h'.
*/
void genera_atributos_proyectil(proyectil **array, int indice, int tipo_proyectil);

/*
Función que asigna los atributos propios de cada tipo de proyectil, hace
uso de la función 'genera_atributos()', recorriendo la matriz en el proceso.
*/
void asigna_proyectiles(int ***Matriz, int filas, int columnas, proyectil **array);

/*
Función que desplaza a un proyectil hacia la derecha, siendo semejante a la función
'desplaza_zombie' de 'entidades.h', con la única diferencia siendo el sentido de
desplazamiento.
*/
void desplazamiento_proyectil(int i, int j, int ***Matriz, int indice, proyectil **array);

#endif