#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "entidades.h"
#include "proyectiles.h"

int encuentra_indice_arreglo(int filas, int columnas, int ***Matriz, int fila_elemento, int columna_elemento) {
	int indice_aux = 0;
	for (int i = 0; i < filas; i++) {
		for (int j = 0; j < columnas; j++) {

			if (i == fila_elemento && j == columna_elemento) 
				return indice_aux;

			else if ((*Matriz)[i][j] != 0)
				indice_aux++;
		}
	}
	return -1;
}

int genera_arreglo_proyectiles(proyectil **array, int cantidad_proyectiles) {
	*array = NULL;

	if (cantidad_proyectiles > 0)	
	(*array) = malloc(cantidad_proyectiles * sizeof(proyectil));	
	
	if ((*array) == NULL)
		return 1;

	return 0;
}

int genera_arreglo_entidades(entidad **array, int cantidad_entidades) {
	*array = NULL;

	if (cantidad_entidades > 0)	
		(*array) = malloc(cantidad_entidades * sizeof(entidad));	
	
	if ((*array) == NULL)
		return 1;

	return 0;
}

void llena_matriz(int ***M, int n, int m) {
	int valor_aux;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%d", &valor_aux);
			(*M)[i][j] = valor_aux;
		}
	}
}

void genera_matriz(int ***M, int n, int m) {
	(*M) = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
		(*M)[i] = malloc(m * sizeof(int));
}

int obtiene_cantidad_tipos_zombies() {
	int cantidad_tipos_zombies = 0;
	char buffer[50];			// buffer para leer las líneas del archivo
	char tipo_entidad_str[15];	// string auxiliar
	char *token; 				// string token auxiliar

	FILE *fh_entidades;	// apertura del stream

	// abrimos 'entidades.txt' en modo lectura
	fh_entidades = fopen("entidades.txt", "r");	

	for (int i = -50; i < 0; i++) {
		sprintf(tipo_entidad_str, "(%d)", i);
		FILE *fh_entidades;	// apertura del stream

		// abrimos 'entidades.txt' en modo lectura
		fh_entidades = fopen("entidades.txt", "r");	

		// En caso de no poder leer el archivo de texto
		if (fh_entidades == NULL) {
			printf("'entidades.txt' no pudo ser abierto..\n");
			return 1;
		}

		while (fgets(buffer, sizeof(buffer), fh_entidades) != NULL) {
			token = strtok(buffer, " \n");
			while (token) {
				if (strcmp(token, tipo_entidad_str) == 0)
					cantidad_tipos_zombies++;
				
				token = strtok(NULL, " \n");
			}
		}
		rewind(fh_entidades);
	}

	fclose(fh_entidades);
	return cantidad_tipos_zombies;
}
int obtiene_proyectil(char atributo[], int tipo_proyectil) {
	char buffer[50];			// buffer para leer las líneas del archivo
	char tipo_proyectil_str[10];	// string auxiliar
	char *token; 				// string token auxiliar
	int bandera = 0;			// flag auxiliar
	int valor_atributo = -1;	// valor del atributo del tipo de entidad

	// Convertimos tipo de entidad a formato del archivo
	sprintf(tipo_proyectil_str, "(%d)", tipo_proyectil);

	FILE *fh_proyectiles;	// apertura del stream

	// abrimos 'proyectiles.txt' en modo lectura
	fh_proyectiles = fopen("proyectiles.txt", "r");	

	// En caso de no poder leer el archivo de texto
	if (fh_proyectiles == NULL) {
		printf("'proyectiles.txt' no pudo ser abierto..\n");
		return 1;
	}

	// Buscamos tipo de entidad
	while (fgets(buffer, sizeof(buffer), fh_proyectiles) != NULL && !bandera) {

		token = strtok(buffer, " \n");

		while (token) {	
			if (strcmp(token, tipo_proyectil_str) == 0) {
				bandera = 1;
				break;
			}
			else
				token = strtok(NULL, " \n");
		}
	}

	// Buscamos atributo
	bandera = 0;
	// Leemos cada linea del archivo
	while (fgets(buffer, sizeof(buffer), fh_proyectiles) != NULL && !bandera) {

		token = strtok(buffer, " \n");

		while (token) {	
			if (strcmp(token, atributo) == 0) {
				// Nos saltamos dos tokens
				token = strtok(NULL, " \n");
				token = strtok(NULL, " \n");

				valor_atributo = atoi(token);
				bandera = 1;
				break;
			} 
			else
				token = strtok(NULL, " \n");
		}
	}

	// Cerramos stream y devolvemos valor encontrado
	fclose(fh_proyectiles);
	return valor_atributo;
}

int obtiene_entidad(char atributo[], int tipo_entidad) {
	char buffer[50];			// buffer para leer las líneas del archivo
	char tipo_entidad_str[10];	// string auxiliar
	char *token; 				// string token auxiliar
	int bandera = 0;			// flag auxiliar
	int valor_atributo = -1;	// valor del atributo del tipo de entidad

	// Convertimos tipo de entidad a formato del archivo
	sprintf(tipo_entidad_str, "(%d)", tipo_entidad);

	FILE *fh_entidades;	// apertura del stream

	// abrimos 'entidades.txt' en modo lectura
	fh_entidades = fopen("entidades.txt", "r");	

	// En caso de no poder leer el archivo de texto
	if (fh_entidades == NULL) {
		printf("'entidades.txt' no pudo ser abierto..\n");
		return 1;
	}

	// Buscamos tipo de entidad
	while (fgets(buffer, sizeof(buffer), fh_entidades) != NULL && !bandera) {

		token = strtok(buffer, " \n");

		while (token) {	
			if (strcmp(token, tipo_entidad_str) == 0) {
				bandera = 1;
				break;
			}
			else
				token = strtok(NULL, " \n");
		}
	}

	// Buscamos atributo
	bandera = 0;
	// Leemos cada linea del archivo
	while (fgets(buffer, sizeof(buffer), fh_entidades) != NULL && !bandera) {

		token = strtok(buffer, " \n");

		while (token) {	
			if (strcmp(token, atributo) == 0) {
				// Nos saltamos dos tokens
				token = strtok(NULL, " \n");
				token = strtok(NULL, " \n");

				valor_atributo = atoi(token);
				bandera = 1;
				break;
			} 
			else
				token = strtok(NULL, " \n");
		}
	}

	// Cerramos stream y devolvemos valor encontrado
	fclose(fh_entidades);
	return valor_atributo;
}
