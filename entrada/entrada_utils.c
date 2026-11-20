#include <stdio.h>
#include <stdlib.h>

#include "entrada_utils.h"

// Funcion para leer el archivo de variables guardadas 
int cargarVariables(const char *nombreArchivo, Variables_Main_Loop *variables) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return 0; // falla
    } 

    // Leemos los 15 numeros de forma ordenada y los guardamos en la caja 
    fscanf(archivo, "%d %f %f %d %d %d %d %d %d %f %d %d %f %f %d\n", 
        &variables->soles, 
        &variables->reloj_juego,
        &variables->reloj_auxiliar,
        &variables->oleada_en_curso,
        &variables->oleadas_transcurridas,

        &variables->presupuesto_oleada,
        &variables->juego_activo,
        &variables->indice_entidad,
        &variables->indice_proyectil,
        &variables->contador_soles,

        &variables->indice_aux,
        &variables->tipo_de_zombie,
        &variables->contador_spawn_oleada,
        &variables->tiempo_spawn_oleada,
        &variables->fila_casilla_spawn);

    fclose(archivo); // cerramos el archivo 
    return 1; // exito 
}

// Guardar el estado actual de las variables del juego
int guardarVariables(const char *nombreArchivo, const Variables_Main_Loop *variables) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        return 0; // falla
    }

    // Guardar cada variable del juego en el archivo
    fprintf(archivo, "%d %f %f %d %d %d %d %d %d %f %d %d %f %f %d\n", 
        variables->soles, 
        variables->reloj_juego,
        variables->reloj_auxiliar,
        variables->oleada_en_curso,
        variables->oleadas_transcurridas,

        variables->presupuesto_oleada,
        variables->juego_activo,
        variables->indice_entidad,
        variables->indice_proyectil,
        variables->contador_soles,

        variables->indice_aux,
        variables->tipo_de_zombie,
        variables->contador_spawn_oleada,
        variables->tiempo_spawn_oleada,
        variables->fila_casilla_spawn);

    fclose(archivo);
    return 1;
}

// Funcion para leer el archivo de configuracion 
int cargarConfiguracion(const char *nombreArchivo, Configuracion *config) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return 0; // falla
    } 

    // Leemos los 7 numeros de forma ordenada y los guardamos en la caja 
    fscanf(archivo, "%d %f %f %d %f %f %d",
           &config->cantidad_soles_generados_automaticamente,
           &config->tiempo_generacion_automatica_soles,
           &config->duracion_tick,
           &config->numero_de_ataques_para_destruir_casa,
           &config->tiempo_preparacion_inicial,
           &config->tiempo_de_descanso,
           &config->cantidad_de_oleadas);

    fclose(archivo); // cerramos el archivo 
    return 1; // exito 
}

// Funcion para leer el archivo del mapa 
int cargarMapa(const char *nombreArchivo, Mapa *mapa) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return 0; // falla 
    }

    // Primero leemos el tamano del mapa (cuantas filas y columnas tiene)
    fscanf(archivo, "%d %d", &mapa->filas, &mapa->columnas);

    // Pedimos espacio en la memoria para las filas y columnas del mapa 
    mapa->matriz = (int **)malloc(mapa->filas * sizeof(int *));
    for (int i = 0; i < mapa->filas; i++) {
        mapa->matriz[i] = (int *)malloc(mapa->columnas * sizeof(int));
    }

    // Leemos el archivo casilla por casilla y guardamos los valores en la matriz
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            fscanf(archivo, "%d", &mapa->matriz[i][j]);
        }
    }

    fclose(archivo);
    return 1;
}

// Liberacion de memoria dinamica 
void liberarMapa(Mapa *mapa) {
    if (mapa->matriz != NULL) {
        for (int i = 0; i < mapa->filas; i++) {
            free(mapa->matriz[i]);
        }
        free(mapa->matriz);
        mapa->matriz = NULL;
    }
}

// Funcion para mostrar el mapa en la pantalla 
void mostrarMapa(const Mapa *mapa) {
    printf("\n--- TABLERO DEL JUEGO ---\n");
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            printf("%d ", mapa->matriz[i][j]);
        }
        printf("\n");
    }
}

// Paquete 1: Guardar el estado actual del mapa en un archivo 
int guardarEstado(const char *nombreArchivo, const Mapa *mapa) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        return 0; // falla
    }

    // Guardar dimensiones del mapa 
    fprintf(archivo, "%d %d\n", mapa->filas, mapa->columnas);
    
    // Guardamos el estado de cada casilla del mapa en el archivo 
    for (int i = 0; i < mapa->filas; i++) {
        for (int j = 0; j < mapa->columnas; j++) {
            fprintf(archivo, "%d ", mapa->matriz[i][j]);
        }
        fprintf(archivo, "\n");      
    } 
    
    fclose(archivo);
    return 1;
}

// Aumentar el tamano del mapa y redimensionar memoria 
void aumentarMatriz(Mapa *mapa, Configuracion *config, int nuevasFilas, int nuevasColumnas) {
    if (nuevasFilas <= mapa->filas && nuevasColumnas <= mapa->columnas) {  
        printf("\nLas nuevas dimensiones deben ser mayores a las actuales.\n");
        return;
    }  

    // Redimensionar las filas 
    mapa->matriz = (int **)realloc(mapa->matriz, nuevasFilas * sizeof(int *));

    // Ajustar columnas de las filas existentes y rellenar nuevas casillas con 0 
    for (int i = 0; i < mapa->filas; i++) {
        mapa->matriz[i] = (int *)realloc(mapa->matriz[i], nuevasColumnas * sizeof(int));
        for (int j = mapa->columnas; j < nuevasColumnas; j++) {
            mapa->matriz[i][j] = 0;
        }
    }

    // Crear las nuevas filas completas con 0 
    for (int i = mapa->filas; i < nuevasFilas; i++) {
        mapa->matriz[i] = (int *)malloc(nuevasColumnas * sizeof(int));
        for (int j = 0; j < nuevasColumnas; j++) {
            mapa->matriz[i][j] = 0;
        }
    }

    // Actualizar dimensiones 
    mapa->filas = nuevasFilas;
    mapa->columnas = nuevasColumnas;

    // REGLA: ajustar la velocidad de generacion de soles al agrandar el mapa
    if (config->tiempo_generacion_automatica_soles > 1) {
        config->tiempo_generacion_automatica_soles--;
    }

    printf("\n-> Mapa agrandado a %dx%d. Tiempo de generacion de soles ajustado a: %.2f.\n",
           mapa->filas, mapa->columnas, config->tiempo_generacion_automatica_soles);
}