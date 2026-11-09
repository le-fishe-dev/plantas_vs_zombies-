#include "entidades.h"
#include "logica_utils.h"

void genera_soles(float *contador_soles, int *soles, int CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE, int TIEMPO_GENERACION_AUTOMATICA_SOLES) {
	// Si el contador alcanza el tiempo suficiente, aumentamos la cantidad de soles
	if ((*contador_soles) >= TIEMPO_GENERACION_AUTOMATICA_SOLES) {
		(*soles) += CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE;
		(*contador_soles) -= TIEMPO_GENERACION_AUTOMATICA_SOLES;
	}
}

void genera_soles_planta(int indice, entidad **array, int *soles) {
	// Si el contador individual de la planta alcanza su tiempo necesario
	// para generar soles, genera su cantidad especificada de soles
	if ((*array)[indice].contador_soles >= (*array)[indice].tiempo_en_generar_soles) {
			(*array)[indice].contador_soles -= (*array)[indice].tiempo_en_generar_soles;
			(*soles) += (*array)[indice].generacion_soles;
	}
}

void desplazamiento_zombie_final(int indice, entidad **array, int *vida_de_la_casa, int *juego_activo) {
	// Si el zombie se encuentra en la primera columna, comenzará a atacar
	// a la casa, quitándole puntos de vida según el daño que haga el zombie
	(*array)[indice].contador_ataque = 0;
	if ((*array)[indice].contador_movimiento >= (*array)[indice].tiempo_en_moverse) {
		(*array)[indice].contador_movimiento -= (*array)[indice].tiempo_en_moverse;
		(*vida_de_la_casa) -= (*array)[indice].ataque;
	}
}

void desplazamiento_zombie(int i, int j, int ***M, int indice, entidad **array) {
	// Si hay una casilla vacía a la izquierda del zombie, este intentará 
	// desplazarse hacia esa casilla, siempre que siga vacía
	(*array)[indice].contador_ataque = 0;
	if ((*array)[indice].contador_movimiento >= (*array)[indice].tiempo_en_moverse) 
	{
		(*array)[indice].contador_movimiento -= (*array)[indice].tiempo_en_moverse;
		(*M)[i][j-1] = (*M)[i][j];
		(*M)[i][j] = 0;
	}
}

void ataca_planta(int indice, entidad **array) {
	// Si hay una planta en la casilla a la izquierda del zombie, el zombie
	// intentará atacar a la planta, siempre que siga habiendo una planta ahí
	(*array)[indice].contador_movimiento = 0;
	if ((*array)[indice].contador_ataque >= (*array)[indice].tiempo_en_atacar)
	{
		(*array)[indice].contador_ataque = 0;
		(*array)[indice - 1].vida -= (*array)[indice].ataque;
	}
}

int es_valido_invocar_zombies(int n, int m, int ***M, int tipo_de_zombie, int *fila_casilla_spawn) {
	int flag_aux = 1;
	int rand_aux = 0;

	//  Buscamos una casilla vacía en la última columna de la matriz de entidades
	for (int i = 0; i < n; i++) {
		if ((*M)[i][m - 1] == 0)
			flag_aux = 0;
	}

	// Si encontramos una casilla vacía, escogemos una posición disponible
	// aleatoria para ubicar o 'spawnear' al zombie en la matriz
	if (flag_aux == 0) {
		rand_aux = rand() % n;
		while ((*M)[rand_aux][m - 1] != 0)
			rand_aux = rand() % n;

		// Almacenamos la fila en donde hicimos aparecer al zombie
		*fila_casilla_spawn = rand_aux;
		(*M)[*fila_casilla_spawn][m - 1] = tipo_de_zombie;
		return 1;
	}
	else
		return 0;
}

int hay_presupuesto_para_zombies(int *presupuesto_oleada, int cantidad_tipos_de_zombies, int *tipo_de_zombie) {
	int rand_aux = 0;
	int costo_zombie = 0;

	// Si existe presupuesto para la oleada, escogemos un zombie aleatorio,
	// con tal que su costo esté dentro del presupuesto. Así, almacenamos al 
	// tipo de zombie que escogimos.
	if (*presupuesto_oleada > 0) {

		// rand_aux es el tipo aleatorio de zombie que estamos escogiendo
		rand_aux = -(rand() % (cantidad_tipos_de_zombies) + 1);
		costo_zombie = obtiene_entidad("costo", rand_aux);

		// si el costo de ese tipo de zombie excede el presupuesto actual,
		// escogemos otro zombie válido
		while (costo_zombie > *presupuesto_oleada) {
			rand_aux = -(rand() % (cantidad_tipos_de_zombies) + 1);
			costo_zombie = obtiene_entidad("costo", rand_aux);
		}

		// Al encontrar un zombie con un costo válido, almacenamos su tipo
		// y actualizamos el monto del presupuesto de la oleada
		*presupuesto_oleada -= costo_zombie;
		*tipo_de_zombie = rand_aux;
		return 1;
	}
	else
		return 0;
}


int hay_tiempo_entre_invocaciones (float *contador_spawn_oleada, float *tiempo_spawn_oleada, float delta_reloj) {
	// Analizamos si hay suficiente tiempo entre cada 'spawn' o
	// aparición de los zombies en la oleada (esto se hace para que
	// no aparezcan todos los zombies "de golpe").
	if ((*contador_spawn_oleada) < (*tiempo_spawn_oleada)) {
		(*contador_spawn_oleada) += delta_reloj;
		return 0;
	}

	// Si no se cumple la distancia de tiempo necesaria entre
	// cada 'spawn', entonces aumentamos el contador.
	else {
		(*contador_spawn_oleada) = 0;
		(*tiempo_spawn_oleada) = (rand() % (2)) + 1;
		return 1;
	}
}

void dispara_proyectil(int filas_matriz, int columnas_matriz, int ***Matriz_Entidades, int ***Matriz_Proyectiles, int *cantidad_proyectiles, proyectil **array_proyectiles, int indice_entidad, entidad **array_entidades,int i, int j) {
	/*
	Esperamos a que el contador de ataque de la planta iguale a su
	tiempo de ataque, y disparamos el proyectil contra el zombie.
	*/
	if ((*array_entidades)[indice_entidad].contador_ataque >= (*array_entidades)[indice_entidad].tiempo_en_atacar){
		(*array_entidades)[indice_entidad].contador_ataque -= (*array_entidades)[indice_entidad].tiempo_en_atacar;
		
		// Creamos un objeto 'proyectil' en la matriz de proyectiles
		if ((*Matriz_Proyectiles)[i][j] == 0) {

			// Generamos el tipo de proyectil correspondiente al tipo de planta
			if ((*Matriz_Entidades)[i][j] == 2) 
				(*Matriz_Proyectiles)[i][j] = 1;	// Proyectil de Lanzaguisantes Común
			
			else if ((*Matriz_Entidades)[i][j] == 4) 
				(*Matriz_Proyectiles)[i][j] = 2; 	// Proyectil de Lanzaguisantes de Hielo

			/* 
			Manejamos el arreglo de proyectiles, generándolo
			en caso de que no exista, o expandiéndolo si ya
			existia anteriormente. 
			*/
			if (*cantidad_proyectiles == 0) {
				(*cantidad_proyectiles)++;
				genera_arreglo_proyectiles(array_proyectiles, *cantidad_proyectiles);
				asigna_proyectiles(Matriz_Proyectiles, filas_matriz, columnas_matriz, array_proyectiles);
			}
			
			else if (*cantidad_proyectiles != 0) {
				int indice_aux = encuentra_indice_arreglo(filas_matriz, columnas_matriz, Matriz_Proyectiles, i, j);
				agrega_proyectil(indice_aux, array_proyectiles, cantidad_proyectiles, (*Matriz_Proyectiles)[i][j]);
			}
		}
	}
}

void impacto_proyectil(int indice_proyectil, proyectil **array_proyectiles, int indice_aux, entidad **array_entidades, int i, int j, int ***Matriz_Entidades) {
	// Si el proyectil está en llamas, hace el doble de daño
	if ((*array_proyectiles)[indice_proyectil].esta_ardiendo == 1)
		(*array_entidades)[indice_aux].vida -= ((*array_proyectiles)[indice_proyectil].ataque * 2);
	else
		(*array_entidades)[indice_aux].vida -= (*array_proyectiles)[indice_proyectil].ataque;

	// Si el proyectil está congelado, hace que al zombie le tome el doble de tiempo caminar
	if ((*array_proyectiles)[indice_proyectil].esta_congelado == 1) {	
		int tiempo_original = obtiene_entidad("tiempo_en_moverse", (*Matriz_Entidades)[i][j]);
		(*array_entidades)[indice_aux].tiempo_en_moverse = tiempo_original * 2;
	}
}