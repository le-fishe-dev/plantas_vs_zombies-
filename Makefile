CC = gcc

CFLAGS = -Wall -Wextra

SDL_FLAGS = $(shell pkg-config --cflags --libs sdl3 sdl3-image)

OBJ = main.o logica/mecanicas_del_juego.o logica/proyectiles.o logica/entidades.o logica/logica_utils.o entrada/entrada_utils.o graficos/graficos_utils.o

plantas_vs_zombies: $(OBJ)
	$(CC) $(OBJ) -o plantas_vs_zombies  $(SDL_FLAGS)

logica/main.o: main.c
	$(CC) $(CFLAGS) -c main.c

logica/mecanicas_del_juego.o: logica/mecanicas_del_juego.c
	$(CC) $(CFLAGS) -c logica/mecanicas_del_juego.c -o logica/mecanicas_del_juego.o

logica/proyectiles.o: logica/proyectiles.c
	$(CC) $(CFLAGS) -c logica/proyectiles.c -o logica/proyectiles.o

logica/entidades.o: logica/entidades.c
	$(CC) $(CFLAGS) -c logica/entidades.c -o logica/entidades.o
	
logica/logica_utils.o: logica/logica_utils.c
	$(CC) $(CFLAGS) -c logica/logica_utils.c -o logica/logica_utils.o

entrada/entrada_utils.o: entrada/entrada_utils.c
	$(CC) $(CFLAGS) -c entrada/entrada_utils.c -o entrada/entrada_utils.o

graficos/graficos_utils.o: graficos/graficos_utils.c
	$(CC) $(CFLAGS) -c graficos/graficos_utils.c -o graficos/graficos_utils.o	

clean:
	rm plantas_vs_zombies main.o logica/*.o entrada/*.o graficos/*.o

play:
	./plantas_vs_zombies