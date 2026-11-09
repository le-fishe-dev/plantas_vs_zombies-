#include <stdio.h>
#include "entidades.h"
#include "logica_utils.h"

void agrega_entidad(int indice, entidad **array, int *cantidad_entidades, int tipo_entidad) {
	// Primero, reajustamos al entero 'cantidad_entidades'
	(*cantidad_entidades) += 1;

	// Luego, reajustamos el tamaño del arreglo para agregar un
	// espacio adicional al final.
	*array = realloc(*array, (*cantidad_entidades) * sizeof(entidad));

	// Posteriormente, movemos todas las entidades, desde el indice,
	// hacia la derecha
	for (int i = *cantidad_entidades - 1; i > indice; i--)
		(*array)[i] = (*array)[i - 1];

	// Finalmente, agregamos la nueva entidad en el índice
	genera_atributos_entidad(array, indice, tipo_entidad);
 }

void elimina_entidad(int indice, entidad **array, int *cantidad_entidades) {
	// Primero, movemos todas las entidades, desde el indice + 1,
	// hacia la izquierda
	for (int i = indice + 1; i < *cantidad_entidades; i++)
		(*array)[i - 1] = (*array)[i];

	// Luego, reajustamos el entero 'cantidad_entidades'
	(*cantidad_entidades) -= 1;

	// Si no quedan entidades, eliminamos el arreglo
	if (*cantidad_entidades == 0) {
		free(*array);
		*array = NULL;
		return;
	}

	// Finalmente, reajustamos el tamaño del arreglo para remover el último
	// indice, el cual está ahora repetido
	*array = realloc(*array, (*cantidad_entidades) * sizeof(entidad));
}

void genera_atributos_entidad(entidad **array, int indice, int tipo_entidad) {
	(*array)[indice].vida = obtiene_entidad("vida", tipo_entidad);

	(*array)[indice].ataque = obtiene_entidad("ataque", tipo_entidad);
	(*array)[indice].tiempo_en_atacar = obtiene_entidad("tiempo_en_atacar", tipo_entidad);
	(*array)[indice].contador_ataque = 0;

	(*array)[indice].tiempo_en_moverse = obtiene_entidad("tiempo_en_moverse", tipo_entidad);
	(*array)[indice].contador_movimiento = 0;

	(*array)[indice].generacion_soles = obtiene_entidad("generacion_soles", tipo_entidad);
	(*array)[indice].tiempo_en_generar_soles = obtiene_entidad("tiempo_en_generar_soles", tipo_entidad);
	(*array)[indice].contador_soles = 0;


	(*array)[indice].costo = obtiene_entidad("costo", tipo_entidad);
}

void asigna_entidades(int ***Matriz, int filas, int columnas, entidad **array) {
	int contador_aux = 0;
	for (int i = 0; i < filas; i++) {	// recorremos filas
		for (int j = 0; j < columnas; j++) {	// recorremos columnas
			if ((*Matriz)[i][j] != 0){	// si se encuentra una entidad
				// definimos atributos de la entidad
				genera_atributos_entidad(array, contador_aux, (*Matriz)[i][j]);
				contador_aux++;
			}
		}
	}
}

int busca_entidades(int ***M, int n, int m, int *cantidad_zombies) {
	int cantidad_entidades = 0;
	(*cantidad_zombies) = 0;	// CAMBIO RECIENTE, aprovechamos de contar zombies

	for (int i = 0; i < n; i++) {	// recorremos filas
		for (int j = 0; j < m; j++) {	// recorremos columnas
			if ((*M)[i][j] != 0) {	// si se encuentra una entidad, aumentamos variable
				cantidad_entidades++;
				if ((*M)[i][j] < 0)	// si es zombie, aumentamos la otra variable
					(*cantidad_zombies)++;
			}
		}
	}
	return cantidad_entidades;
}