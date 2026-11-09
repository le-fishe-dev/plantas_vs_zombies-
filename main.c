#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "entidades.h"
#include "proyectiles.h"
#include "mecanicas_del_juego.h"
#include "logica_utils.h"

#define CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE 25  // 25 es la cantidad estándar del juego
#define TIEMPO_GENERACION_AUTOMATICA_SOLES 10.00     // Debe ser alto para cumplir la lógica exigida del proyecto
#define DURACION_TICK 0.05                           // Recomendado entre 0.05 y 0.10  (1.00 para mayor estabilidad) 
#define NUMERO_DE_ATAQUES_PARA_DESTRUIR_CASA 1       // Debe ser positivo
#define TIEMPO_ENTRE_OLEADAS 40.00                   // Menos de 40 (aprox) hace que no hayan descansos entre oleadas
#define TIEMPO_PREPARACION_INICIAL 18.00             // NO CAMBIAR ESTE PARÁMETRO, valor exigido por proyecto
#define TIEMPO_DURACION_DE_UNA_OLEADA 10.00          // Alrededor de 10 es razonable

int main(void) {
	/* ---------------- LECTURA DE LA ENTRADA ---------------- */
	
	// Ingresar matriz aquí
	int **Matriz_Entidades;

	// Ingresar filas y columnas de la matriz aquí
	int filas_matriz;	
	int columnas_matriz ;	

	/* ------------------- LÓGICA DEL JUEGO ------------------ */

	// Obtiene seed para el generador de números aleatorios
	srand(time(NULL));

	// Estructura necesaria para usar la función nanosleep()
	const struct timespec tiempo_tick = {
		.tv_sec = (time_t)DURACION_TICK,
		.tv_nsec = (long)((DURACION_TICK - (time_t)DURACION_TICK) * 1000000000L),
	};

	/*
	Generamos una matriz idéntica a la entregada, la cual servirá para
	manejar el comportamiento de los proyectiles del juego.
	Nótese como la Matriz de Proyectiles tiene el mismo número de filas
	y columnas que la Matriz de Entidades, esto ocurre porque es una
	copia idéntica, la cual alberga proyectiles, en vez de entidades.
	*/
	int **Matriz_Proyectiles;
	int cantidad_proyectiles = 0;	
	genera_matriz(&Matriz_Proyectiles, filas_matriz, columnas_matriz);

	/*
	Llenamos a la matriz de proyectiles con ceros, para así eliminar
	los datos basura presentes en la matriz, producto de usar malloc.
	*/
	for (int i = 0; i < filas_matriz; i++) {
		for (int j = 0; j < columnas_matriz; j++) {
			Matriz_Proyectiles[i][j] = 0;
		}
	}

	/*
	También declaramos puntero nulo hacia arreglo de proyectiles.
	*/
	proyectil *array_proyectiles;

	/*
	Hacemos un recuento de la cantidades de entidades (plantas o zombies)
	presentes dentro de la matriz y la almacenamos en la variable homónima.
	También se hace un recuento específico de los zombies, para servir de guía
	para la lógica de las oleadas del juego.
	*/
	int cantidad_zombies = 0;
	int cantidad_tipos_de_zombies = obtiene_cantidad_tipos_zombies();
	int cantidad_entidades = busca_entidades(&Matriz_Entidades, filas_matriz, 
		columnas_matriz, &cantidad_zombies);

	/*
	Declaramos puntero asociado a un arreglo dinámico que almacenará
	los datos de las entidades iniciales, siempre que hayan entidades 
	en la matriz inicial.
	*/
	entidad *array_entidades = NULL;
	if (cantidad_entidades > 0)
		genera_arreglo_entidades(&array_entidades, cantidad_entidades);

	/*
	Asignamos los datos correspondientes a cada entidad almacenada
	en el arreglo, según las especificaciones de 'entidades.txt'.
	*/
	asigna_entidades(&Matriz_Entidades, filas_matriz, 
		columnas_matriz, &array_entidades);

	/*
	### Variables Fundamentales del Main Loop ###
	(para agregar un guardado de partida, estas son 
	las variables que deberían ser almacenadas)
	*/
	int soles = 0;
	float reloj_juego = 0; 
	float reloj_auxiliar = 0;
	float delta_reloj = reloj_juego - reloj_auxiliar;
	int flag_oleada = 0;
	int oleadas_transcurridas = 0;
	int presupuesto_oleada = 0;
	int juego_activo = 1;
	int vida_de_la_casa = NUMERO_DE_ATAQUES_PARA_DESTRUIR_CASA;

	// ### Variables Auxiliares del Main Loop ###

	int indice_entidad = 0;
	int indice_proyectil = 0;
	float contador_soles = 0;
	int indice_aux = 0;
	int tipo_de_zombie = 0;
	float contador_spawn_oleada = 0;
	float tiempo_spawn_oleada = 0;
	int fila_casilla_spawn = 0;

	// ###### Main Loop ######

	while (juego_activo == 1){

		/* Definición de 'delta_reloj'
		Es la diferencia de tiempo en el reloj entre la iteración actual del 
		main loop y la iteración anterior. Es la base fundamental de todos 
		los ciclos de tiempo de las entidades y los proyectiles.
		*/
		delta_reloj = reloj_juego - reloj_auxiliar;

		/* Inicio de una oleada
		La primera oleada del juego comienza a los 18 segundos, luego, cada
		oleada subsiguiente tendrá una diferencia determinada entre si.
		OJO: La duración por defecto entre oleadas esta ajustada para que solo hayan cuatro oleadas.
		*/
		if ((int)reloj_juego == TIEMPO_PREPARACION_INICIAL + (TIEMPO_ENTRE_OLEADAS * oleadas_transcurridas + (oleadas_transcurridas)*(oleadas_transcurridas + 2)) && oleadas_transcurridas < 4) {
			oleadas_transcurridas++;
			flag_oleada = oleadas_transcurridas;

			// Fórmula del término general para la secuencia 2, 5, 9, 14, ...
			presupuesto_oleada += ((flag_oleada * flag_oleada) + (3 * flag_oleada)) / 2;
		}

		/* Fin de una oleada
		El primer descanso entre oleadas iniciará al finalizar la primera oleada, y cada 
		descanso subsiguiente tendra una diferencia determinada entre si.
		*/
		if (reloj_juego == (TIEMPO_PREPARACION_INICIAL + TIEMPO_DURACION_DE_UNA_OLEADA) + (TIEMPO_ENTRE_OLEADAS * (oleadas_transcurridas - 1) + (oleadas_transcurridas)*(oleadas_transcurridas + 2))) {
			flag_oleada = 0;
			presupuesto_oleada = 0;
		}

		/* Logica para agregar zombies en una oleada 
		Es necesario tener un tiempo cada vez que se agregan, que exista un 
		presupuesto lo suficientemente adecuado para pagar el 'costo' de 
		cada tipo de zombie y que existan casillas vacías disponibles en la 
		última columna para ubicarlos en ese lugar.
		*/
		if (hay_tiempo_entre_invocaciones(&contador_spawn_oleada, &tiempo_spawn_oleada, delta_reloj)
			&& hay_presupuesto_para_zombies(&presupuesto_oleada, cantidad_tipos_de_zombies, &tipo_de_zombie) 
			&& es_valido_invocar_zombies(filas_matriz, columnas_matriz, &Matriz_Entidades, tipo_de_zombie, &fila_casilla_spawn) ) {
			cantidad_zombies++;
			if (cantidad_entidades == 0) {
				cantidad_entidades++;
				genera_arreglo_entidades(&array_entidades, cantidad_entidades);
				asigna_entidades(&Matriz_Entidades, filas_matriz, columnas_matriz, &array_entidades);
			}
			
			else if (cantidad_entidades != 0) {
				indice_aux = encuentra_indice_arreglo(filas_matriz, columnas_matriz, 
					&Matriz_Entidades, fila_casilla_spawn, columnas_matriz - 1);
				agrega_entidad(indice_aux, &array_entidades, &cantidad_entidades, tipo_de_zombie);
			}
		}

		/*
		Generación automática de soles:
		Cada cierta cantidad de segundos, se genera una determinada cantidad de soles
		automáticamente, estando ambas variables definidas dentro de la configuración
		inicial del programa.
		*/
		contador_soles += delta_reloj;
		genera_soles(&contador_soles, &soles, 
			CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE, 
			TIEMPO_GENERACION_AUTOMATICA_SOLES);

		/*
		Loop de recorrido de la matriz de entidades:
		Se recorre cada una de las casilla de la matriz de entidades, realizando las
		siguientes tareas:

		1. Eliminar entidades sin puntos de vida
		2. Generar soles, a partir de plantas productoras de energía
		3. Invocar proyectiles, a partir de plantas ofensivas
		4. Atacar la casa, a partir de zombies en la primera columna
		5. Desplazarse a la casilla izquierda, aplica solo para zombies
		6. Atacar a la planta en la casilla izquierda, también para zombies
		*/
		indice_entidad = 0;
		indice_proyectil = 0;
		for (int i = 0; i < filas_matriz; i++) {

			/* Lógica de las Entidades
			Se analiza de izquierda a derecha, pues es en el 
			sentido opuesto al desplazamiento de los zombies.
			*/
			for (int j = 0; j < columnas_matriz; j++) {

				if (Matriz_Entidades[i][j] != 0) {	// Al encontrar una entidad en la matriz

					/* Entidad sin vida
					Al encontrar una entidad sin vida, se elimina 
					de la matriz y del arreglo de entidades.
					*/
					if (array_entidades[indice_entidad].vida <= 0) {
						if (Matriz_Entidades[i][j] < 0)
							cantidad_zombies--;
						Matriz_Entidades[i][j] = 0;
						elimina_entidad(indice_entidad, 
							&array_entidades, &cantidad_entidades);
						if (indice_entidad > 0)
							indice_entidad--;
					}

					/* Comportamiento de las Plantas
					Al encontrar una planta, se analizan sus atributos.
					*/
					else if (Matriz_Entidades[i][j] > 0) {

						/* Planta generadora de Soles
						La planta intenta generar soles en base a su 
						ciclo de tiempo correspondiente.
						*/
						if (array_entidades[indice_entidad].generacion_soles > 0) {
							array_entidades[indice_entidad].contador_soles += delta_reloj;
							genera_soles_planta(indice_entidad, &array_entidades, &soles);
						}

						/* Planta ofensiva
						La planta revisa si hay zombies a su derecha. De ser verdadero, 
						invoca un proyectil en la matriz de proyectiles, estando ubicada 
						en la misma posición que la suya.
						*/
						else if (array_entidades[indice_entidad].ataque > 0) {
							indice_aux = indice_entidad;

							int hay_zombie_en_fila = 0;
							for (int k = j + 1; k < columnas_matriz; k++) {
								/*
								Si encontramos un zombie en la misma fila que la planta
								ofensiva, entonces intentamos disparar un proyectil.
								*/
								if (Matriz_Entidades[i][k] < 0) {
									hay_zombie_en_fila = 1;
									array_entidades[indice_entidad].contador_ataque += delta_reloj;
									dispara_proyectil(filas_matriz, columnas_matriz, 
										&Matriz_Entidades, &Matriz_Proyectiles,
										&cantidad_proyectiles, &array_proyectiles,
										indice_entidad, &array_entidades, i, j);
									break;
								}
							}
							/*
							Si la planta ofensiva no encuentra ningún zombie dentro de su fila,
							entonces reinicia su contador de ataque, quedando así en 'reposo'.
							*/
							if (hay_zombie_en_fila == 0)
								array_entidades[indice_entidad].contador_ataque = 0;
						}
					}

					/* Comportamiento de los Zombies
					Al encontrar un zombie, se analizan sus casillas adyacentes.
					*/
					else if (Matriz_Entidades[i][j] < 0) {	
						/* Caso 1: Zombie ubicado en primera columna
						El zombie comienza a atacar la casa, reduciendo su vida.
						Si la vida de la casa llega a cero, el juego acaba en 
						una derrota para el jugador.
						*/
						if (j == 0) {
							array_entidades[indice_entidad].contador_movimiento += delta_reloj;
							desplazamiento_zombie_final(indice_entidad, &array_entidades, &vida_de_la_casa, &juego_activo);
						}

						/* Caso 2: Casilla libre a la izquierda del zombie
						Si hay una casilla libre a la izquierda del zombie, el
						zombie intentará desplazarse hacia esa casilla.
						*/
						else if (Matriz_Entidades[i][j-1] == 0) {
							array_entidades[indice_entidad].contador_movimiento += delta_reloj;
							desplazamiento_zombie(i, j, &Matriz_Entidades, indice_entidad, &array_entidades);
						}

						/* Caso 3: Zombie encuentra planta directamente a su izquierda
						Si hay una planta a la izquierda del zombie, el zombie comenzará
						a atacarla, reduciendo su vida.
						*/
						else if (Matriz_Entidades[i][j-1] > 0) {
							array_entidades[indice_entidad].contador_ataque += delta_reloj;
							ataca_planta(indice_entidad, &array_entidades);
						}

						/* Caso 4: Zombie encuentra otro zombie a su izquierda
						El zombie espera a que el otro zombie cambie de casilla. 
						Ojo: como la lectura de entidades se hace en sentido contrario
						al del desplazamiento de los zombies, esto no provocará problemas.
						*/
					}
					indice_entidad++;
				}
			}
			/* Lógica de los Proyectiles
			Se analiza de derecha a izquierda, pues es en el 
			sentido opuesto al desplazamiento de los proyectiles.
			*/
			for (int j = columnas_matriz - 1; j >= 0; j--) {
				if (Matriz_Proyectiles[i][j] != 0) {
					// Obtenemos el índice correcto del proyectil (pues al leer aquí de derecha a izquierda,
					// el orden del índice puede ser distinto a como fue inicializado en el bucle anterior)
					indice_proyectil = encuentra_indice_arreglo(filas_matriz, columnas_matriz, &Matriz_Proyectiles, i, j);

					// Proyectil se encuentra con un zombie
					if (Matriz_Entidades[i][j] < 0) {
						indice_aux = encuentra_indice_arreglo(filas_matriz, columnas_matriz, &Matriz_Entidades, i, j);

						impacto_proyectil(indice_proyectil, &array_proyectiles, indice_aux, &array_entidades, i, j, &Matriz_Entidades);

						Matriz_Proyectiles[i][j] = 0;
						elimina_proyectil(indice_proyectil, &array_proyectiles, &cantidad_proyectiles);
					}

					// Si el proyectil no se encontró con un zombie, seguirá su rumbo..
					else {
						// Proyectil se encuentra con un 'Torchwood' (el tronco con una llama encima)
						if (Matriz_Entidades[i][j] == 5) {
							array_proyectiles[indice_proyectil].esta_ardiendo = 1;
							array_proyectiles[indice_proyectil].esta_congelado = 0;
						}

						// Proyectil se encuentra en la última columna
						if (j == columnas_matriz - 1) {
							Matriz_Proyectiles[i][j] = 0;
							elimina_proyectil(indice_proyectil, &array_proyectiles, &cantidad_proyectiles);
						}

						// Proyectil tiene una casilla vacía a su derecha
						else if (Matriz_Proyectiles[i][j + 1] == 0) {
							array_proyectiles[indice_proyectil].contador_para_recorrer_casilla += delta_reloj;
							desplazamiento_proyectil(i, j, &Matriz_Proyectiles, indice_proyectil, &array_proyectiles);						
						}
					}
				}
			}
		}

		/* Condición de victoria!
		Si el jugador finaliza la última oleada y no quedan
		más zombies en el tablero, entonces el juego termina 
		en una victoria para el jugador.
		*/
		if (oleadas_transcurridas == 4 && flag_oleada == 0 && cantidad_zombies == 0)
			juego_activo = 2;

		/* Condición de derrota..
		Si los zombies llegan a la primera columna de la matriz
		y comienzan a atacar la casa, el juego termina cuando
		la vida de la casa llegue a cero.
		*/
		if (vida_de_la_casa <= 0)
			juego_activo = -1;

		/* ------------------- VISUALIZACIÓN DEL MAPA ------------------ */

		// Mostrar pantalla aquí


		/* ------------------- MÁS LÓGICA DEL JUEGO ------------------ */
		/* Lógica del Tiempo
		El juego se pausa durante la duración de un tick
		para permitir el procesado del programa. La duración
		del tick puede ser modificada, pero se recomienda un
		valor de entre 0.10 a 0.05 segundos.
		*/
		if (juego_activo == 1){	
			nanosleep(&tiempo_tick, NULL);
			reloj_auxiliar = reloj_juego;			
			reloj_juego += DURACION_TICK;	
		}
	}

	if (juego_activo == 2) 	// Victoria del jugador
		printf("\n\n   ¡Has ganado!\n\n");
	
	else if (juego_activo == -1) 	// Derrota del jugador
		printf("\n\n   Lástima, has perdido.\n\n");
	
	// Liberamos los arreglos y matrices de memoría dinámica
	for (int i = 0; i < filas_matriz; i++) {
		free(Matriz_Entidades[i]);
		free(Matriz_Proyectiles[i]);
	}

	// Temporal, para volver a mostrar el cursor
	printf("\033[?25h");

	// Liberamos la memoria dinámica
	free(Matriz_Entidades);		
	free(array_entidades);				
	free(Matriz_Proyectiles);
	free(array_proyectiles);
	
	return 0;
}