#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "logica/entidades.h"
#include "logica/proyectiles.h"
#include "logica/mecanicas_del_juego.h"
#include "logica/logica_utils.h"

#include "entrada/entrada_utils.h"

#include "graficos/graficos_utils.h"

int main(void) {
	/* ---------------- INICIALIZACIÓN GRÁFICA ---------------- */

	// Se revisa si SDL está correctamente instalado.
	if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
        return 1;
    }

	/* ---------------- LECTURA DE LA ENTRADA ---------------- */
	
	Configuracion miconfig;
    Mapa mimapa;
    Variables_Main_Loop misvariables;

    int opcion = 0;

    // Variables sujetas a posibilidad de cargar partida guardada
    // Asignamos valor por defecto de estas variables
	misvariables.soles = 0;
	misvariables.reloj_juego = 0.0f; 
	misvariables.reloj_auxiliar = 0.0f;
	misvariables.oleada_en_curso = 0;
	misvariables.oleadas_transcurridas = 0;

	misvariables.presupuesto_oleada = 0;
	misvariables.juego_activo = 1;
	misvariables.indice_entidad = 0;
	misvariables.indice_proyectil = 0;
	misvariables.contador_soles = 0.0f;

	misvariables.indice_aux = 0;
	misvariables.tipo_de_zombie = 0;
	misvariables.contador_spawn_oleada = 0.0f;
	misvariables.tiempo_spawn_oleada = 0.0f;
	misvariables.fila_casilla_spawn = 0;

    // Cargar la configuracion desde el archivo
    if (cargarConfiguracion("configuracion/configuracion.txt", &miconfig)) {
        printf("Configuracion cargada exitosamente:\n\n");

        printf("Cantidad de soles generados automaticamente: %d\n", miconfig.cantidad_soles_generados_automaticamente);
        printf("Tiempo de generacion automatica de soles: %.2f\n", miconfig.tiempo_generacion_automatica_soles);
        printf("Duracion de un tick: %.2f\n", miconfig.duracion_tick);
        printf("Numero minimo de ataques para destruir casa: %d\n", miconfig.numero_de_ataques_para_destruir_casa);
        printf("Tiempo de preparacion inicial: %.2f\n", miconfig.tiempo_preparacion_inicial);
        printf("Tiempo de descanso: %.2f\n", miconfig.tiempo_de_descanso);
        printf("Cantidad de oleadas: %d\n", miconfig.cantidad_de_oleadas);
    } else {
        printf("Error al cargar la configuracion.\n");
        return 1;
    }

    // Cargar mapa inicial 
    if (!cargarMapa("configuracion/mapa.txt", &mimapa)) {
        printf("Error al cargar el mapa.\n");
        return 1;
    }

    // Menu interactivo con las funciones del paquete 
    do {
        mostrarMapa(&mimapa);
        printf("\n--- MENU PAQUETE 1 ---\n");
        printf("1. Cargar partida guardada\n");
        printf("2. Aumentar tamano del mapa\n");
        printf("3. Jugar!\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        
        // Instrucción para despejar la terminal
		printf("\033[2J\033[H");

        if (opcion == 1) {
            liberarMapa(&mimapa); // liberar mapa previo 
            if (cargarMapa("configuracion/partida_guardada.txt", &mimapa)) {
                printf("\nPartida cargada exitosamente!\n");
            } else {
                printf("\nError al cargar la partida guardada.\n");
            }
            if (!cargarVariables("configuracion/variables_guardadas.txt", &misvariables)) {
        		printf("Error al cargar las variables.\n");
        		return 1;
    		}
        } else if (opcion == 2) {
            int nf, nc;
            printf("Ingresa nuevas filas y columnas (actual: %dx%d): ", mimapa.filas, mimapa.columnas);
            scanf("%d %d", &nf, &nc);
            while(getchar() != '\n');
            aumentarMatriz(&mimapa, &miconfig, nf, nc);
        }

    } while (opcion != 3);

	// Se genera matriz de 5x9 con memoria dinámica.
	int **Matriz_Entidades = mimapa.matriz;	// puntero de matriz
	int filas_matriz = mimapa.filas;	
	int columnas_matriz = mimapa.columnas;	

	// Se asignan datos de la configuración leida
	int CANTIDAD_SOLES_GENERADOS_AUTOMATICAMENTE = miconfig.cantidad_soles_generados_automaticamente;
	float TIEMPO_GENERACION_AUTOMATICA_SOLES = miconfig.tiempo_generacion_automatica_soles;
	float DURACION_TICK = miconfig.duracion_tick;
	int NUMERO_DE_ATAQUES_PARA_DESTRUIR_CASA = miconfig.numero_de_ataques_para_destruir_casa;      
	float TIEMPO_PREPARACION_INICIAL = miconfig.tiempo_preparacion_inicial;
	float TIEMPO_DE_DESCANSO = miconfig.tiempo_de_descanso;
	int CANTIDAD_DE_OLEADAS = miconfig.cantidad_de_oleadas;

	// Actualizamos variables con el valor asignado final
	int soles = misvariables.soles;
	float reloj_juego = misvariables.reloj_juego; 
	float reloj_auxiliar = misvariables.reloj_auxiliar;
	int oleada_en_curso = misvariables.oleada_en_curso;
	int oleadas_transcurridas = misvariables.oleadas_transcurridas;

	int presupuesto_oleada = misvariables.presupuesto_oleada;
	int juego_activo = misvariables.juego_activo ;
	int indice_entidad = misvariables.indice_entidad;
	int indice_proyectil = misvariables.indice_proyectil;
	float contador_soles = misvariables.contador_soles;

	int indice_aux = misvariables.indice_aux;
	int tipo_de_zombie = misvariables.tipo_de_zombie;
	float contador_spawn_oleada = misvariables.contador_spawn_oleada;
	float tiempo_spawn_oleada = misvariables.tiempo_spawn_oleada;
	int fila_casilla_spawn = misvariables.fila_casilla_spawn;

	// Variables independientes
	int vida_de_la_casa = NUMERO_DE_ATAQUES_PARA_DESTRUIR_CASA;
	float tiempo_inicio_de_oleada = (float)TIEMPO_PREPARACION_INICIAL;
	float delta_reloj = 0.0f;

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
	proyectil *array_proyectiles = NULL;

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

	// Instrucción para despejar la terminal
	printf("\033[2J\033[H");

	// Instrucción para esconder el cursor
	printf("\033[?25l");

	/* ---------------- INICIALIZACIÓN GRÁFICA ---------------- */

	// Crear ventana
	SDL_Window* window = SDL_CreateWindow(
	    "Plants vs Zombies",
	    ANCHO_VENTANA,
	    ALTO_VENTANA,
	    SDL_WINDOW_RESIZABLE
	);

	if (window == NULL) {
	    SDL_Log("Error al crear ventana: %s", SDL_GetError());
	    SDL_Quit();
	    return 1;
	}

	// Crear renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	if (renderer == NULL) {
	    SDL_Log("Error al crear renderer: %s", SDL_GetError());
	    SDL_DestroyWindow(window);
	    SDL_Quit();
	    return 1;
	}

	// Cargar texturas después de inicializar el renderer
	TexturasJuego tex = cargar_texturas(renderer);

	// ###### Main Loop ######

    SDL_Event evento;
    int guardar_solicitado = 0; // Tecla G: Guardar partida
	while (juego_activo == 1){

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_EVENT_QUIT) {
                juego_activo = 0;
            }
            // Detectar tecla G (Guardar partida) una sola vez por pulsacion
            else if (evento.type == SDL_EVENT_KEY_DOWN && !evento.key.repeat) {
                if (evento.key.scancode == SDL_SCANCODE_G) {
                    guardar_solicitado = 1;
                }
            }
        }


		/* Definición de 'delta_reloj'
		Es la diferencia de tiempo en el reloj entre la iteración actual del 
		main loop y la iteración anterior. Es la base fundamental de todos 
		los ciclos de tiempo de las entidades y los proyectiles.
		*/
		delta_reloj = reloj_juego - reloj_auxiliar;

		/* Inicio de una oleada
		Se asigna 2 a 'oleada_en_curso' para indicar que recien
		acaba de empezar una oleada, y se debe agregar el presupuesto
		para la nueva oleada de zombies.
		*/
		if (oleada_en_curso == 0 && reloj_juego >= tiempo_inicio_de_oleada)
			oleada_en_curso = 2;

		/*
		Si en una oleada no hay zombies y no hay presupuesto, se analizan 
		las posibles explicaciones: la oleada acaba de empezar o acaba de 
		terminar.
		*/
		if (oleada_en_curso != 0 && presupuesto_oleada == 0 && cantidad_zombies == 0) {
			switch(oleada_en_curso) {
				/* Fin de una oleada / Inicio de un descanso
				Comienza un descanso de 'TIEMPO_DE_DESCANSO' segundos.
				*/
				case 1:
					oleadas_transcurridas++;
					oleada_en_curso = 0;
					presupuesto_oleada = 0;
					tiempo_inicio_de_oleada = reloj_juego + TIEMPO_DE_DESCANSO;
					break;
				/* Inicio de una oleada
				Se agrega un monto al presupuesto de la oleada. Este monto
				será el producto entre 3 y el nivel de la oleada.
				*/
				case 2:
					oleada_en_curso = 1;
					presupuesto_oleada += 3 * (oleadas_transcurridas + 1);
					break;
			}
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
							desplazamiento_zombie_final(indice_entidad, &array_entidades, &vida_de_la_casa);
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
		Si el jugador finaliza la última oleada, entonces 
		el juego termina en una victoria para el jugador.
		*/
		if (oleadas_transcurridas == CANTIDAD_DE_OLEADAS && oleada_en_curso == 0)
			juego_activo = 2;

		/* Condición de derrota..
		Si los zombies llegan a la primera columna de la matriz
		y comienzan a atacar la casa, el juego termina cuando
		la vida de la casa llegue a cero.
		*/
		if (vida_de_la_casa <= 0)
			juego_activo = -1;

		// ----Representación Gráfica----

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        dibujar_tablero(renderer, &tex, filas_matriz, columnas_matriz, Matriz_Entidades, Matriz_Proyectiles);

        SDL_RenderPresent(renderer);

		// Se imprime matriz actual
		printf("\033[H");

		printf("\n\n   Soles: %d\n", soles);

		printf("   Oleadas transcurridas: %d\n", oleadas_transcurridas);

		printf("   Inicio de la oleada: %f\n", tiempo_inicio_de_oleada);

		printf("   Presupuesto de la oleada : %d\n", presupuesto_oleada);

		printf("   Oleada en curso: %d\n", oleada_en_curso);

		printf("   Reloj del juego: %.2f\n\n", reloj_juego);

		printf("\t Matriz de Entidades\n");
		for (int i = 0; i < filas_matriz; i++) {
			printf("\t  ");
			for (int j = 0; j < columnas_matriz; j++){
				printf("%d ", Matriz_Entidades[i][j]);
			}
			printf("\n");
		}

		printf("\n");

		printf("\tMatriz de Proyectiles\n");
		for (int i = 0; i < filas_matriz; i++) {
			printf("\t  ");
			for (int j = 0; j < columnas_matriz; j++) {
				printf("%d ", Matriz_Proyectiles[i][j]);
			}
			printf("\n");
		}

		printf("\n");

		/* Guardado de Partida
		Se almacena el mapa del juego actual y las variables que sean
		necesarias para conservar la logica del juego.
		*/
        if (guardar_solicitado) {
            guardar_solicitado = 0;

            // Abrimos stream
            FILE *fh = fopen("log_guardado_de_partida.txt", "w");
            if (fh == NULL) 
            	return 1;	// Error al crear archivo

            // Guardamos la matriz actual en el archivo correspondiente de texto
            if (guardarEstado("configuracion/partida_guardada.txt", &mimapa)) {
                fprintf(fh, "Partida guardada exitosamente en 'configuracion/partida_guardada.txt'!\n");
            } else {
                fprintf(fh, "Error al guardar la partida.\n");
            }

            // Actualizamos las variables
			misvariables.soles = soles;
			misvariables.reloj_juego = reloj_juego;
			misvariables.reloj_auxiliar = reloj_auxiliar;
			misvariables.oleada_en_curso = oleada_en_curso;
			misvariables.oleadas_transcurridas = oleadas_transcurridas;

			misvariables.presupuesto_oleada = presupuesto_oleada;
			misvariables.juego_activo = juego_activo;
			misvariables.indice_entidad = indice_entidad;
			misvariables.indice_proyectil = indice_proyectil;
			misvariables.contador_soles = contador_soles;

			misvariables.indice_aux = indice_aux;
			misvariables.tipo_de_zombie = tipo_de_zombie;
			misvariables.contador_spawn_oleada = contador_spawn_oleada;
			misvariables.tiempo_spawn_oleada = tiempo_spawn_oleada;
			misvariables.fila_casilla_spawn = fila_casilla_spawn;

			// Las guardamos en el archivo correspondiente de texto
            if (guardarVariables("configuracion/variables_guardadas.txt", &misvariables)) {
                fprintf(fh, "Variables guardadas exitosamente en 'configuracion/variables_guardadas.txt'!\n");
            } else {
                fprintf(fh, "Error al guardar las variables.\n");
            }

            // Cerramos stream
            fclose(fh);
        }

		/* Lógica de Tiempo
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

	// Liberamos memoria asociada a la representación gráfica de SDL3
    liberar_texturas(&tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

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
