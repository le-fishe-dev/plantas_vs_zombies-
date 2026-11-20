#ifndef GRAFICOS_UTILS_H
#define GRAFICOS_UTILS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define ANCHO_VENTANA 1100
#define ALTO_VENTANA 550

// Configuracion de las hojas de sprites animadas de zombies
#define COLS_Z_CAMINAR         5
#define FILAS_Z_CAMINAR        10
#define FRAMES_Z_CAMINAR       46

#define COLS_Z_COMER           5
#define FILAS_Z_COMER          8
#define FRAMES_Z_COMER         40

#define COLS_Z_CONO            5
#define FILAS_Z_CONO           13
#define FRAMES_Z_CONO          63

#define COLS_Z_BANDERA         5
#define FILAS_Z_BANDERA        18
#define FRAMES_Z_BANDERA       90

#define COLS_Z_BANDERA_COMER   5
#define FILAS_Z_BANDERA_COMER  25
#define FRAMES_Z_BANDERA_COMER 121

typedef struct {
    SDL_Texture* jardin;
    SDL_Texture* pasto_respaldo;
    SDL_Texture* zombie_caminar;       // zombie_caminar.png (ID -1 caminar, 5x10)
    SDL_Texture* zombie_comer;         // zombie_comer.png (comer general, 5x8)
    SDL_Texture* zombie_respaldo;      // zombie.png (respaldo estatico)
    SDL_Texture* zombie_bandera;       // zombie_bandera.png / zombie_bandera_caminar.png (ID -2 caminar, 5x18)
    SDL_Texture* zombie_bandera_comer; // zombie_bandera_comer.png (ID -2 comer, 5x25)
    SDL_Texture* zombie_cono;          // zombie_cono.png (ID -3 caminar, 5x13)
    SDL_Texture* zombie_balde;         // zombie_balde.png (ID -4)
    SDL_Texture* girasol;              // girasol.png (ID 1, 5x5)
    SDL_Texture* girasol_doble;        // girasoldoble.png (ID 6, 5x4)
    SDL_Texture* lanzaguisantes;       // lanzaguisantes.png (ID 2, 5x5)
    SDL_Texture* lanza_hielo;          // lanzaguisanteshielo.png (ID 4, 5x5)
    SDL_Texture* nuez;                 // nuez.png (ID 3, 5x4)
    SDL_Texture* tronco;               // tronco.png (ID 5, 5x12)
    SDL_Texture* guisante;             // guisante.png (Proyectil 1)
    SDL_Texture* guisante_azul;        // guisanteazul.png (Proyectil 2)
} TexturasJuego;

// Carga todas las texturas del juego (llamar al inicio de main.c)
TexturasJuego cargar_texturas(SDL_Renderer* renderer);

// Libera toda la memoria grafica al cerrar el programa
void liberar_texturas(TexturasJuego* tex);

// Dibuja un sprite manteniendo su proporcion original, con altura uniforme y apoyado en la base de la celda
void dibujar_sprite_igualado(
    SDL_Renderer* renderer,
    SDL_Texture* textura,
    SDL_FRect* src_rect,
    float celda_x,
    float celda_y,
    float tam_celda,
    float escala_altura,
    float offset_x_rel);

// Nota: Si en el main.c las matrices son punteros dobles (int **),
// cambiar "int matriz_entidades[filas][columnas]" por "int **matriz_entidades"
// Nota del editor: Esto es cierto, pues nuestras matrices se almacenan en el heap.
void dibujar_tablero(
    SDL_Renderer* renderer,
    TexturasJuego* tex,
    int filas,
    int columnas,
    int **matriz_entidades,
    int **matriz_proyectiles);

#endif