#ifndef ENTIDADES_H
#define ENTIDADES_H
#include <stdio.h>
#include <stdlib.h>

/*
Estructura que almacena los datos de cada entidad del juego.
*/
typedef struct Entidad
{
	int vida;						// puntos de vida (HP)

	int ataque;						// solo para zombies y plantas ofensivas

	float tiempo_en_atacar;			// en ataques por segundo
	float contador_ataque;			// en segundos

	float tiempo_en_moverse;		// solo para zombies
	float contador_movimiento;		// en segundos

	int generacion_soles;			// solo para plantas energéticas

	float tiempo_en_generar_soles;  // en segundos
	float contador_soles;			// en segundos

	int costo;						// solo para las plantas

} entidad;

/*
Función que agrega una nueva entidad en el arreglo de entidades,
siendo especificado el indice donde se desea agregar la nueva entidad y
el tipo de entidad a agregar.
*/
void agrega_entidad(int indice, entidad **array, int *cantidad_entidades, int tipo_entidad);

/*
Función que elimina una entidad del arreglo de entidades, siendo
especificado el indice de la entidad que se desea eliminar.
*/
void elimina_entidad(int indice, entidad **array, int *cantidad_entidades);

/*
Función que directamente define los valores enteros de cada atributo
de una entidad, obteniendo aquel valor entero desde la funcion 'obtiene()',
definida en el header 'logica_utils.h'.
*/
void genera_atributos_entidad(entidad **array, int indice, int tipo_entidad);

/*
Función que asigna los atributos propios de cada tipo de entidad, hace
uso de la función 'genera_atributos()', recorriendo la matriz en el proceso.
*/
void asigna_entidades(int ***Matriz, int filas, int m, entidad **array);

/*
Función que hace un recuento de la cantidad de entidades presentes
en la matriz. Hace un recuento específico de los zombies para asistir
en la lógica de las oleadas del juego.
*/
int busca_entidades(int ***M, int n, int m, int *cantidad_zombies);

#endif