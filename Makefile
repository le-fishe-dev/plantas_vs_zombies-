plantas_vs_zombies: main.c
	gcc -Wall -g3 -fsanitize=address -o plantas_vs_zombies main.c entidades.c logica_utils.c mecanicas_del_juego.c proyectiles.c

clean:
	rm plantas_vs_zombies
