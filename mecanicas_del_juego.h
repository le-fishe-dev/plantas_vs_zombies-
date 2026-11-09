#ifndef MECANICAS_DEL_JUEGO_H
#define MECANICAS_DEL_JUEGO_H
#include "entidades.h"

/*
Función que genera una cantidad determinada de soles automáticamente, cada cierta cantidad
determinada de segundos, siendo estas definidas al inicio del juego. Esta función responde
a los requerimientos establecidos en el Canvas para la versión simplificada del juego.
*/
void genera_soles(float *contador_soles, int *soles, 
	int CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE, int TIEMPO_GENERACION_AUTOMATICA_SOLES);

/*
Función que genera una cantidad determinada de soles por parte de una planta productora
de soles, siendo ambas definidas en la sección correspondiente del archivo 'entidades.txt' 
de la planta en particular.
*/
void genera_soles_planta(int indice, entidad **array, int *soles);

/*
Función que permite al zombie desplazarse una última vez, una vez llegado a la columna 
final del juego. Si el zombie se desplaza y no hay nadie que lo detenga, el juego termina
en una derrota para el jugador.
*/
void desplazamiento_zombie_final(int indice, entidad **array, int* vida_de_la_casa, int *juego_activo);

/*
Función que permite al zombie desplazarse hacia una casilla despejada, siempre y cuando siga
despejada. Si el jugador agrega una planta en frente del zombie, mientras este intentaba
desplazarse a la misma casilla, el contador se reinicia y el zombie comenzará a atacar
a la planta.
*/
void desplazamiento_zombie(int i, int j, int ***M, int indice, entidad **array);

/*
Función que permite al zombie atacar a una planta que se encuentre justo en frente de este,
siempre y cuando la planta siga existiendo, puesto que el jugador podría remover la planta
con una pala (posible elemento de jugabilidad). Si la planta es eliminada y la casilla se 
encuentra despejada, entonces el zombie inicia su proceso para desplazarse a esa casilla.
*/
void ataca_planta(int indice, entidad **array);

/*
Función booleana que devuelve TRUE si existe una casilla vacía en donde agregar 
a un zombie nuevo en la última columna de la matriz, y devuelve FALSE si no hay 
ninguna casilla libre en la última columna de la matriz. 
NOTA: Crucialmente, también es la encargada de agregar al tipo de zombie 
específico a la casilla de la matriz en cuestión.
*/
int es_valido_invocar_zombies(int n, int m, int ***M, int tipo_de_zombie, int *fila_casilla_spawn);

/*
Función booleana que devuelve TRUE si hay presupuesto en la oleada para invocar a
más zombies, siendo el mínimo 1 para poder invocar al zombie más básico. Devuelve
FALSE si no hay presupuesto para invocar a ningún zombie más.
*/
int hay_presupuesto_para_zombies(int *presupuesto_oleada, int cantidad_tipos_de_zombies, int *tipo_de_zombie);

/*
Función booleana que devuelve TRUE si se cumplió una cantidad de tiempo aleatoria,
entre 1 y 2 segundos, para poder volver a invocar a un zombie durante una oleada.
*/
int hay_tiempo_entre_invocaciones (float *contador_spawn_oleada, float *tiempo_spawn_oleada, float delta_reloj);

/*
Función que maneja los siguientes elementos del proyecto:
1. Crear y actualizar el arreglo de proyectiles
2. Agregar proyectiles a la matriz de proyectiles
*/
void dispara_proyectil(int filas_matriz, int columnas_matriz, 
	int ***Matriz_Entidades, int ***Matriz_Proyectiles, 
	int *cantidad_proyectiles, proyectil **array_proyectiles, 
	int indice_entidad, entidad **array_entidades, 
	int i, int j);

/*
Función que maneja el daño generado al zombie tras recibir el impacto de un proyectil.
También analiza los modificadores del proyectil, tales como estar congelado o estar en
llamas, lo cual se ve traducido en distintos impactos sobre el zombie (recibe el doble
de daño o su velocidad se ralentiza a la mitad).
*/
void impacto_proyectil(int indice_proyectil, proyectil **array_proyectiles, int indice_aux, entidad **array_entidades, int i, int j, int ***Matriz_Entidades);


#endif

