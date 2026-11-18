#ifndef GRAFICOS_UTILS_H
#define GRAFICOS_UTILS_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define ANCHO_VENTANA 1100
#define ALTO_VENTANA 550

// Configuracion de las hojas de sprites de zombies
#define COLS_Z_CAMINAR   9
#define FILAS_Z_CAMINAR  1
#define FRAMES_Z_CAMINAR 9

#define COLS_Z_COMER     5
#define FILAS_Z_COMER    8
#define FRAMES_Z_COMER   40

typedef struct {
    SDL_Texture* jardin;
    SDL_Texture* pasto_respaldo;
    SDL_Texture* zombie_caminar;
    SDL_Texture* zombie_comer;
    SDL_Texture* zombie_respaldo;
    SDL_Texture* zombieb;
    SDL_Texture* girasol;
    SDL_Texture* lanzaguisantes;
    SDL_Texture* lanza_hielo;
    SDL_Texture* nuez;
    SDL_Texture* tronco;
    SDL_Texture* guisante;
    SDL_Texture* guisante_azul;
} TexturasJuego;

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

void dibujar_tablero(
    SDL_Renderer* renderer,
    TexturasJuego* tex,
    int filas,
    int columnas,
    int **matriz_entidades,
    int **matriz_proyectiles);

#endif