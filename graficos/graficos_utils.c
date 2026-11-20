#include "graficos_utils.h"

// Carga todas las texturas del juego (llamar al inicio de main.c)
TexturasJuego cargar_texturas(SDL_Renderer* renderer) {
    TexturasJuego tex = {0};

    tex.jardin               = IMG_LoadTexture(renderer, "Imagenes/jardin.png");
    tex.pasto_respaldo       = IMG_LoadTexture(renderer, "Imagenes/pasto.png");

    // Zombies animados y diferenciados por ID (-1, -2, -3, -4)
    tex.zombie_caminar       = IMG_LoadTexture(renderer, "Imagenes/zombie_caminar.png");
    if (!tex.zombie_caminar) {
        tex.zombie_caminar   = IMG_LoadTexture(renderer, "Imagenes/zombie_caminando.png");
    }
    tex.zombie_comer         = IMG_LoadTexture(renderer, "Imagenes/zombie_comer.png");
    tex.zombie_respaldo      = IMG_LoadTexture(renderer, "Imagenes/zombie.png");

    tex.zombie_bandera       = IMG_LoadTexture(renderer, "Imagenes/zombie_bandera.png");
    if (!tex.zombie_bandera) {
        tex.zombie_bandera   = IMG_LoadTexture(renderer, "Imagenes/zombie_bandera_caminar.png");
    }
    tex.zombie_bandera_comer = IMG_LoadTexture(renderer, "Imagenes/zombie_bandera_comer.png");
    tex.zombie_cono          = IMG_LoadTexture(renderer, "Imagenes/zombie_cono.png");
    tex.zombie_balde         = IMG_LoadTexture(renderer, "Imagenes/zombie_balde.png");

    // Plantas (IDs 1 al 6)
    tex.girasol              = IMG_LoadTexture(renderer, "Imagenes/girasol.png");
    tex.girasol_doble        = IMG_LoadTexture(renderer, "Imagenes/girasoldoble.png");
    tex.lanzaguisantes       = IMG_LoadTexture(renderer, "Imagenes/lanzaguisantes.png");
    tex.lanza_hielo          = IMG_LoadTexture(renderer, "Imagenes/lanzaguisanteshielo.png");
    tex.nuez                 = IMG_LoadTexture(renderer, "Imagenes/nuez.png");
    tex.tronco               = IMG_LoadTexture(renderer, "Imagenes/tronco.png");

    // Proyectiles
    tex.guisante             = IMG_LoadTexture(renderer, "Imagenes/guisante.png");
    tex.guisante_azul        = IMG_LoadTexture(renderer, "Imagenes/guisanteazul.png");

    return tex;
}

// Libera toda la memoria grafica al cerrar el programa
void liberar_texturas(TexturasJuego* tex) {
    if (!tex) return;
    if (tex->jardin) SDL_DestroyTexture(tex->jardin);
    if (tex->pasto_respaldo) SDL_DestroyTexture(tex->pasto_respaldo);
    if (tex->zombie_caminar) SDL_DestroyTexture(tex->zombie_caminar);
    if (tex->zombie_comer) SDL_DestroyTexture(tex->zombie_comer);
    if (tex->zombie_respaldo) SDL_DestroyTexture(tex->zombie_respaldo);
    if (tex->zombie_bandera) SDL_DestroyTexture(tex->zombie_bandera);
    if (tex->zombie_bandera_comer) SDL_DestroyTexture(tex->zombie_bandera_comer);
    if (tex->zombie_cono) SDL_DestroyTexture(tex->zombie_cono);
    if (tex->zombie_balde) SDL_DestroyTexture(tex->zombie_balde);
    if (tex->girasol) SDL_DestroyTexture(tex->girasol);
    if (tex->girasol_doble) SDL_DestroyTexture(tex->girasol_doble);
    if (tex->lanzaguisantes) SDL_DestroyTexture(tex->lanzaguisantes);
    if (tex->lanza_hielo) SDL_DestroyTexture(tex->lanza_hielo);
    if (tex->nuez) SDL_DestroyTexture(tex->nuez);
    if (tex->tronco) SDL_DestroyTexture(tex->tronco);
    if (tex->guisante) SDL_DestroyTexture(tex->guisante);
    if (tex->guisante_azul) SDL_DestroyTexture(tex->guisante_azul);
}

// Dibuja un sprite manteniendo su proporcion original, con altura uniforme y apoyado en la base de la celda
void dibujar_sprite_igualado(
    SDL_Renderer* renderer,
    SDL_Texture* textura,
    SDL_FRect* src_rect,
    float celda_x,
    float celda_y,
    float tam_celda,
    float escala_altura,
    float offset_x_rel
) {
    if (!textura) return;

    float frame_w, frame_h;
    if (src_rect != NULL) {
        frame_w = src_rect->w;
        frame_h = src_rect->h;
    } else {
        SDL_GetTextureSize(textura, &frame_w, &frame_h);
    }

    if (frame_h <= 0.0f) return;

    float alto_final  = tam_celda * escala_altura;
    float ancho_final = alto_final * (frame_w / frame_h);

    float dst_x = celda_x + (tam_celda - ancho_final) / 2.0f + (tam_celda * offset_x_rel);
    float dst_y = (celda_y + tam_celda) - alto_final;

    SDL_FRect rect_destino = { dst_x, dst_y, ancho_final, alto_final };
    SDL_RenderTexture(renderer, textura, src_rect, &rect_destino);
}

// Nota: Si en el main.c de tu equipo las matrices son punteros dobles (int **),
// cambia "int matriz_entidades[filas][columnas]" por "int **matriz_entidades"
void dibujar_tablero(
    SDL_Renderer* renderer,
    TexturasJuego* tex,
    int filas,
    int columnas,
    int **matriz_entidades,
    int **matriz_proyectiles) {
    // 1. Calcular tamano automatico de cada celda para el mapa modular
    float max_celda_y = (float)ALTO_VENTANA / (filas + 0.85f);
    float max_celda_x = (float)ANCHO_VENTANA / (columnas + 1.05f * (filas + 0.85f));
    float tam_celda = (max_celda_x < max_celda_y) ? max_celda_x : max_celda_y;

    float alto_cerca      = tam_celda * 0.85f;
    float alto_pasto      = filas * tam_celda;
    float alto_total_mapa = alto_cerca + alto_pasto;

    float ancho_casa       = alto_total_mapa * 0.40f;
    float ancho_pasto      = columnas * tam_celda;
    float ancho_calle      = alto_total_mapa * 0.65f;
    float ancho_total_mapa = ancho_casa + ancho_pasto + ancho_calle;

    float inicio_x = (ANCHO_VENTANA - ancho_total_mapa) / 2.0f;
    float inicio_y = (ALTO_VENTANA  - alto_total_mapa) / 2.0f;

    float pasto_x = inicio_x + ancho_casa;
    float pasto_y = inicio_y + alto_cerca;

    // 2. Dibujar casa, cerca y calle desde jardin.png
    SDL_FRect src_pasto_oscuro = {0};
    SDL_FRect src_pasto_claro  = {0};
    bool usar_recorte_jardin = false;

    if (tex->jardin) {
        float jw, jh;
        SDL_GetTextureSize(tex->jardin, &jw, &jh);
        usar_recorte_jardin = true;

        SDL_FRect src_casa = { 0.0f, 0.0f, jw * 0.175f, jh };
        SDL_FRect dst_casa = { inicio_x, inicio_y, ancho_casa, alto_total_mapa };
        SDL_RenderTexture(renderer, tex->jardin, &src_casa, &dst_casa);

        SDL_FRect src_cerca = { jw * 0.175f, 0.0f, jw * 0.540f, jh * 0.14f };
        SDL_FRect dst_cerca = { pasto_x, inicio_y, ancho_pasto, alto_cerca };
        SDL_RenderTexture(renderer, tex->jardin, &src_cerca, &dst_cerca);

        SDL_FRect src_calle = { jw * 0.715f, 0.0f, jw * 0.285f, jh };
        SDL_FRect dst_calle = { pasto_x + ancho_pasto, inicio_y, ancho_calle, alto_total_mapa };
        SDL_RenderTexture(renderer, tex->jardin, &src_calle, &dst_calle);

        float celda_orig_w = (jw * 0.530f) / 9.0f;
        float celda_orig_h = (jh * 0.790f) / 5.0f;
        src_pasto_oscuro = (SDL_FRect){ jw * 0.180f, jh * 0.160f, celda_orig_w, celda_orig_h };
        src_pasto_claro  = (SDL_FRect){ jw * 0.180f + celda_orig_w, jh * 0.160f, celda_orig_w, celda_orig_h };
    }

    // 3. Calcular fotogramas actuales para plantas y zombies
    Uint32 tiempo_actual = SDL_GetTicks();
    const int COLUMNAS_SPRITE = 5;

    int frame_plantas         = (tiempo_actual / 80)  % 24;
    int frame_girasol_doble   = (tiempo_actual / 80)  % 20;
    int frame_nuez            = (tiempo_actual / 100) % 17;
    int frame_tronco          = (tiempo_actual / 60)  % 56;
    int frame_z_caminar       = (tiempo_actual / 75)  % FRAMES_Z_CAMINAR;
    int frame_z_comer         = (tiempo_actual / 55)  % FRAMES_Z_COMER;
    int frame_z_cono          = (tiempo_actual / 75)  % FRAMES_Z_CONO;
    int frame_z_bandera       = (tiempo_actual / 70)  % FRAMES_Z_BANDERA;
    int frame_z_bandera_comer = (tiempo_actual / 45)  % FRAMES_Z_BANDERA_COMER;

    SDL_FRect src_plantas = {0};
    if (tex->girasol) {
        float w, h;
        SDL_GetTextureSize(tex->girasol, &w, &h);
        src_plantas = (SDL_FRect){
            (frame_plantas % COLUMNAS_SPRITE) * (w / 5.0f),
            (frame_plantas / COLUMNAS_SPRITE) * (h / 5.0f),
            w / 5.0f, h / 5.0f
        };
    }

    SDL_FRect src_girasol_doble = {0};
    if (tex->girasol_doble) {
        float w, h;
        SDL_GetTextureSize(tex->girasol_doble, &w, &h);
        src_girasol_doble = (SDL_FRect){
            (frame_girasol_doble % COLUMNAS_SPRITE) * (w / 5.0f),
            (frame_girasol_doble / COLUMNAS_SPRITE) * (h / 4.0f),
            w / 5.0f, h / 4.0f
        };
    }

    SDL_FRect src_nuez = {0};
    if (tex->nuez) {
        float w, h;
        SDL_GetTextureSize(tex->nuez, &w, &h);
        src_nuez = (SDL_FRect){
            (frame_nuez % COLUMNAS_SPRITE) * (w / 5.0f),
            (frame_nuez / COLUMNAS_SPRITE) * (h / 4.0f),
            w / 5.0f, h / 4.0f
        };
    }

    SDL_FRect src_tronco = {0};
    if (tex->tronco) {
        float w, h;
        SDL_GetTextureSize(tex->tronco, &w, &h);
        src_tronco = (SDL_FRect){
            (frame_tronco % COLUMNAS_SPRITE) * (w / 5.0f),
            (frame_tronco / COLUMNAS_SPRITE) * (h / 12.0f),
            w / 5.0f, h / 12.0f
        };
    }

    SDL_FRect src_z_caminar = {0};
    if (tex->zombie_caminar) {
        float w, h;
        SDL_GetTextureSize(tex->zombie_caminar, &w, &h);
        src_z_caminar = (SDL_FRect){
            (frame_z_caminar % COLS_Z_CAMINAR) * (w / (float)COLS_Z_CAMINAR),
            (frame_z_caminar / COLS_Z_CAMINAR) * (h / (float)FILAS_Z_CAMINAR),
            w / (float)COLS_Z_CAMINAR,
            h / (float)FILAS_Z_CAMINAR
        };
    }

    SDL_FRect src_z_comer = {0};
    if (tex->zombie_comer) {
        float w, h;
        SDL_GetTextureSize(tex->zombie_comer, &w, &h);
        src_z_comer = (SDL_FRect){
            (frame_z_comer % COLS_Z_COMER) * (w / (float)COLS_Z_COMER),
            (frame_z_comer / COLS_Z_COMER) * (h / (float)FILAS_Z_COMER),
            w / (float)COLS_Z_COMER,
            h / (float)FILAS_Z_COMER
        };
    }

    SDL_FRect src_z_cono = {0};
    if (tex->zombie_cono) {
        float w, h;
        SDL_GetTextureSize(tex->zombie_cono, &w, &h);
        src_z_cono = (SDL_FRect){
            (frame_z_cono % COLS_Z_CONO) * (w / (float)COLS_Z_CONO),
            (frame_z_cono / COLS_Z_CONO) * (h / (float)FILAS_Z_CONO),
            w / (float)COLS_Z_CONO,
            h / (float)FILAS_Z_CONO
        };
    }

    SDL_FRect src_z_bandera = {0};
    if (tex->zombie_bandera) {
        float w, h;
        SDL_GetTextureSize(tex->zombie_bandera, &w, &h);
        src_z_bandera = (SDL_FRect){
            (frame_z_bandera % COLS_Z_BANDERA) * (w / (float)COLS_Z_BANDERA),
            (frame_z_bandera / COLS_Z_BANDERA) * (h / (float)FILAS_Z_BANDERA),
            w / (float)COLS_Z_BANDERA,
            h / (float)FILAS_Z_BANDERA
        };
    }

    SDL_FRect src_z_bandera_comer = {0};
    if (tex->zombie_bandera_comer) {
        float w, h;
        SDL_GetTextureSize(tex->zombie_bandera_comer, &w, &h);
        src_z_bandera_comer = (SDL_FRect){
            (frame_z_bandera_comer % COLS_Z_BANDERA_COMER) * (w / (float)COLS_Z_BANDERA_COMER),
            (frame_z_bandera_comer / COLS_Z_BANDERA_COMER) * (h / (float)FILAS_Z_BANDERA_COMER),
            w / (float)COLS_Z_BANDERA_COMER,
            h / (float)FILAS_Z_BANDERA_COMER
        };
    }

    // 4. Dibujar tablero casilla por casilla
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            float x = pasto_x + (j * tam_celda);
            float y = pasto_y + (i * tam_celda);
            SDL_FRect rect_celda = { x, y, tam_celda, tam_celda };

            // Pasto
            if (usar_recorte_jardin) {
                SDL_FRect* cuadro_pasto = ((i + j) % 2 == 0) ? &src_pasto_oscuro : &src_pasto_claro;
                SDL_RenderTexture(renderer, tex->jardin, cuadro_pasto, &rect_celda);
            } else if (tex->pasto_respaldo) {
                SDL_RenderTexture(renderer, tex->pasto_respaldo, NULL, &rect_celda);
            }

            // Entidades
            int id_entidad = matriz_entidades[i][j];

            if (id_entidad == 1) {
                if (tex->girasol) SDL_RenderTexture(renderer, tex->girasol, &src_plantas, &rect_celda);
            }
            else if (id_entidad == 6) {
                if (tex->girasol_doble) {
                    SDL_RenderTexture(renderer, tex->girasol_doble, &src_girasol_doble, &rect_celda);
                } else if (tex->girasol) {
                    SDL_FRect g_izq = { x, y + tam_celda * 0.12f, tam_celda * 0.72f, tam_celda * 0.72f };
                    SDL_FRect g_der = { x + tam_celda * 0.28f, y + tam_celda * 0.18f, tam_celda * 0.72f, tam_celda * 0.72f };
                    SDL_RenderTexture(renderer, tex->girasol, &src_plantas, &g_izq);
                    SDL_RenderTexture(renderer, tex->girasol, &src_plantas, &g_der);
                }
            }
            else if (id_entidad == 2) {
                if (tex->lanzaguisantes) SDL_RenderTexture(renderer, tex->lanzaguisantes, &src_plantas, &rect_celda);
            }
            else if (id_entidad == 4) {
                if (tex->lanza_hielo) SDL_RenderTexture(renderer, tex->lanza_hielo, &src_plantas, &rect_celda);
            }
            else if (id_entidad == 5) {
                if (tex->tronco) SDL_RenderTexture(renderer, tex->tronco, &src_tronco, &rect_celda);
            }
            else if (id_entidad == 3) {
                if (tex->nuez) SDL_RenderTexture(renderer, tex->nuez, &src_nuez, &rect_celda);
            }
            else if (id_entidad < 0) {
                // Verifica si tiene una planta a su izquierda (ID 1 a 6)
                int entidad_izq = (j > 0) ? matriz_entidades[i][j - 1] : 0;
                bool esta_comiendo = (entidad_izq >= 1 && entidad_izq <= 6);

                if (esta_comiendo) {
                    if (id_entidad == -2 && tex->zombie_bandera_comer) {
                        dibujar_sprite_igualado(renderer, tex->zombie_bandera_comer, &src_z_bandera_comer, x, y, tam_celda, 1.35f, -0.10f);
                    } else if (tex->zombie_comer) {
                        dibujar_sprite_igualado(renderer, tex->zombie_comer, &src_z_comer, x, y, tam_celda, 1.15f, -0.10f);
                    }
                }
                else if (id_entidad == -2 && tex->zombie_bandera) {
                    dibujar_sprite_igualado(renderer, tex->zombie_bandera, &src_z_bandera, x, y, tam_celda, 1.35f, 0.0f);
                }
                else if (id_entidad == -3 && tex->zombie_cono) {
                    dibujar_sprite_igualado(renderer, tex->zombie_cono, &src_z_cono, x, y, tam_celda, 1.15f, 0.0f);
                }
                else if (id_entidad == -4 && tex->zombie_balde) {
                    dibujar_sprite_igualado(renderer, tex->zombie_balde, NULL, x, y, tam_celda, 1.15f, 0.0f);
                }
                else if (tex->zombie_caminar) {
                    dibujar_sprite_igualado(renderer, tex->zombie_caminar, &src_z_caminar, x, y, tam_celda, 1.15f, 0.0f);
                }
                else if (tex->zombie_respaldo) {
                    dibujar_sprite_igualado(renderer, tex->zombie_respaldo, NULL, x, y, tam_celda, 1.15f, 0.0f);
                }
            }

            // Proyectiles
            int id_proyectil = matriz_proyectiles[i][j];
            if (id_proyectil != 0) {
                float tam_proj = tam_celda * 0.375f;
                float margen_proj = (tam_celda - tam_proj) / 2.0f;
                SDL_FRect rect_proyectil = { x + margen_proj, y + margen_proj, tam_proj, tam_proj };

                if (id_proyectil == 2 || id_entidad == 4) {
                    if (tex->guisante_azul) SDL_RenderTexture(renderer, tex->guisante_azul, NULL, &rect_proyectil);
                } else {
                    if (tex->guisante) SDL_RenderTexture(renderer, tex->guisante, NULL, &rect_proyectil);
                }
            }
        }
    }
}
