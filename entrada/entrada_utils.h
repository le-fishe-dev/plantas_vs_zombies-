#ifndef ENTRADA_H
#define ENTRADA_H
#include <stdio.h>
#include <stdlib.h>

// 1° caja de configuracion 
typedef struct {
    int cantidad_soles_generados_automaticamente;   // 25 es la cantidad estándar del juego
    float tiempo_generacion_automatica_soles;       // Debe ser alto para cumplir la lógica exigida del proyecto
    float duracion_tick;                            // Recomendado entre 0.05 y 0.10  (1.00 para mayor estabilidad) 
    int numero_de_ataques_para_destruir_casa;       // Debe ser entero positivo
    float tiempo_preparacion_inicial;               // DEBE SER 18.00, valor exigido por proyecto
    float tiempo_de_descanso;                       // Debe ser decimal positivo
    int cantidad_de_oleadas;                        // Debe ser entero positivo
} Configuracion;

// 2° caja del mapa jardin 
typedef struct {
    int filas; 
    int columnas;
    int **matriz; // tablero del juego
} Mapa;

// 3° caja de variables del main loop 
typedef struct {
        int soles;
        float reloj_juego; 
        float reloj_auxiliar;
        int oleada_en_curso;
        int oleadas_transcurridas;

        int presupuesto_oleada;
        int juego_activo;
        int indice_entidad;
        int indice_proyectil;
        float contador_soles;

        int indice_aux;
        int tipo_de_zombie;
        float contador_spawn_oleada;
        float tiempo_spawn_oleada;
        int fila_casilla_spawn;
} Variables_Main_Loop;

// Funcion para leer el archivo de variables guardadas 
int cargarVariables(const char *nombreArchivo, Variables_Main_Loop *variables);

// Guardar el estado actual de las variables del juego
int guardarVariables(const char *nombreArchivo, const Variables_Main_Loop *variables);

// Funcion para leer el archivo de configuracion 
int cargarConfiguracion(const char *nombreArchivo, Configuracion *config);

// Funcion para leer el archivo del mapa 
int cargarMapa(const char *nombreArchivo, Mapa *mapa);

// Liberacion de memoria dinamica 
void liberarMapa(Mapa *mapa);

// Funcion para mostrar el mapa en la pantalla 
void mostrarMapa(const Mapa *mapa);

// Paquete 1: Guardar el estado actual del mapa en un archivo 
int guardarEstado(const char *nombreArchivo, const Mapa *mapa);

// Aumentar el tamano del mapa y redimensionar memoria 
void aumentarMatriz(Mapa *mapa, Configuracion *config, int nuevasFilas, int nuevasColumnas);

#endif