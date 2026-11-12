#include <stdio.h>
#include <stdlib.h>

// 1° caja de configuracion 
typedef struct {
    int max_oleadas;
    int vida_casa;
    int vida_zombi_base;
    int freq_disparo;
    int freq_soles;
} Configuracion;

// 2° caja del mapa jardin 
typedef struct {
    int filas; 
    int columnas;
    int **matriz; // tablero del juego
} Mapa;

// Funcion para leer el archivo de configuracion 
int cargarConfiguracion(const char *nombreArchivo, Configuracion *config) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        return 0; // falla
    } 

    // Leemos los 5 numeros de forma ordenada y los guardamos en la caja 
    fscanf(archivo, "%d %d %d %d %d",
           &config->max_oleadas,
           &config->vida_casa,
           &config->vida_zombi_base,
           &config->freq_disparo,
           &config->freq_soles);

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
    if (config->freq_soles > 1) {
        config->freq_soles--;
    }

    printf("\n-> Mapa agrandado a %dx%d. Tasa de soles ajustada a %d.\n",
           mapa->filas, mapa->columnas, config->freq_soles);
}

int main() {
    Configuracion miconfig;
    Mapa mimapa;
    int opcion = 0;

    // Cargar la configuracion desde el archivo
    if (cargarConfiguracion("configuracion.txt", &miconfig)) {
        printf("Configuracion cargada exitosamente:\n");
        printf("Max Oleadas: %d\n", miconfig.max_oleadas);
        printf("Vida Casa: %d\n", miconfig.vida_casa);
        printf("Vida Zombi Base: %d\n", miconfig.vida_zombi_base);
        printf("Frecuencia Disparo: %d\n", miconfig.freq_disparo);
        printf("Frecuencia Soles: %d\n", miconfig.freq_soles);
    } else {
        printf("Error al cargar la configuracion.\n");
        return 1;
    }

    // Cargar mapa inicial 
    if (!cargarMapa("mapa.txt", &mimapa)) {
        printf("Error al cargar el mapa.\n");
        return 1;
    }

    // Menu interactivo con las funciones del paquete 
    do {
        mostrarMapa(&mimapa);
        printf("\n--- MENU PAQUETE 1 ---\n");
        printf("1. Guardar partida actual\n");
        printf("2. Cargar partida guardada\n");
        printf("3. Aumentar tamano del mapa\n");
        printf("4. Salir\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            if (guardarEstado("partida_guardada.txt", &mimapa)) {
                printf("\nPartida guardada exitosamente en 'partida_guardada.txt'!\n");
            } else {
                printf("\nError al guardar la partida.\n");
            }
        } else if (opcion == 2) {
            liberarMapa(&mimapa); // liberar mapa previo 
            if (cargarMapa("partida_guardada.txt", &mimapa)) {
                printf("\nPartida cargada exitosamente!\n");
            } else {
                printf("\nError al cargar la partida guardada.\n");
            }
        } else if (opcion == 3) {
            int nf, nc;
            printf("Ingresa nuevas filas y columnas (actual: %dx%d): ", mimapa.filas, mimapa.columnas);
            scanf("%d %d", &nf, &nc);
            aumentarMatriz(&mimapa, &miconfig, nf, nc);
        }

    } while (opcion != 4);

    liberarMapa(&mimapa);
    return 0; 
}