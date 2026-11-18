#include <stdio.h>
#include "proyectiles.h"
#include "logica_utils.h"

void agrega_proyectil(int indice, proyectil **array, int *cantidad_proyectiles, int tipo_proyectil) {
	if (indice < 0 || indice > *cantidad_proyectiles)
		return;

	// Primero, reajustamos al entero 'cantidad_entidades'
	int cantidad_nueva = *cantidad_proyectiles + 1;

	// Usamos arreglo temporal para asegurarnos que el puntero no esté vacío
	proyectil *temporal = realloc(*array, cantidad_nueva * sizeof(proyectil));

	if (temporal == NULL) {
		(*cantidad_proyectiles)--;
		return;
	}

	*array = temporal;
	*cantidad_proyectiles = cantidad_nueva;

	// Posteriormente, movemos todas las entidades, desde el indice,
	// hacia la derecha
	for (int i = cantidad_nueva - 1; i > indice; i--)
		(*array)[i] = (*array)[i - 1];

	// Finalmente, agregamos la nueva entidad en el índice
	genera_atributos_proyectil(array, indice, tipo_proyectil);
 }

void elimina_proyectil(int indice, proyectil **array, int *cantidad_proyectiles) {
	// Si no hay proyectiles, entonces no hay nada que eliminar
	if (*cantidad_proyectiles <= 0)
		return;

	// Primero, movemos todas los proyectiles del arreglo, desde el indice + 1,
	// hacia la izquierda
	for (int i = indice + 1; i < *cantidad_proyectiles; i++)
		(*array)[i - 1] = (*array)[i];

	// Luego, reajustamos el entero 'cantidad_proyectiles'
	(*cantidad_proyectiles) -= 1;

	// Si no quedan proyectiles, eliminamos el arreglo
	if (*cantidad_proyectiles == 0) {
		free(*array);
		*array = NULL;
		return;
	}

	// Finalmente, reajustamos el tamaño del arreglo para remover el último
	// indice, el cual está ahora repetido
	*array = realloc(*array, (*cantidad_proyectiles) * sizeof(proyectil));
}

void genera_atributos_proyectil(proyectil **array, int indice, int tipo_proyectil) {
	(*array)[indice].ataque = obtiene_proyectil("ataque", tipo_proyectil);
	(*array)[indice].tiempo_en_recorrer_casilla = obtiene_proyectil("tiempo_en_recorrer_casilla", tipo_proyectil);
	(*array)[indice].contador_para_recorrer_casilla = 0;
	(*array)[indice].esta_congelado = obtiene_proyectil("esta_congelado", tipo_proyectil);
	(*array)[indice].esta_ardiendo = 0;
}

void asigna_proyectiles(int ***Matriz, int filas, int columnas, proyectil **array) {
	int contador_aux = 0;
	for (int i = 0; i < filas; i++) {	// recorremos filas
		for (int j = 0; j < columnas; j++) {	// recorremos columnas
			if ((*Matriz)[i][j] != 0){	// si se encuentra un proyectil
				// definimos atributos del proyectil
				genera_atributos_proyectil(array, contador_aux, (*Matriz)[i][j]);
				contador_aux++;
			}
		}
	}
}

void desplazamiento_proyectil(int i, int j, int ***Matriz, int indice, proyectil **array){
	// Si el contador de desplazamiento propio del proyectil alcanza el valor del tiempo
	// necesario para desplazarse, entonces movemos el proyectil hacia la derecha
	if ((*array)[indice].contador_para_recorrer_casilla >= (*array)[indice].tiempo_en_recorrer_casilla) 
	{
		(*array)[indice].contador_para_recorrer_casilla -= (*array)[indice].tiempo_en_recorrer_casilla;
		(*Matriz)[i][j+1] = (*Matriz)[i][j];
		(*Matriz)[i][j] = 0;
	}
}