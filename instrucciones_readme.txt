###########--------- Declaración de Uso de la IA ---------###########
Entrada: No hay registro de uso de la IA.
Lógica: Uso de ChatGPT para encontrar bugs y explicar aspectos del lenguaje C.
Salida: Uso de IA para redactar código.

###########--------- Bugs Conocidos ---------###########
1. Agregar plantas a la última columna provocará que los zombies no puedan spawnear en esa casilla.
2. La animación de ataque del zombie con cono es idéntica a la animación de ataque del zombie común.
3. El presupuesto de la oleada se gasta aún cuando se intenta spawnear un zombie en una casilla ocupada.

###########--------- Guardado de Partida ---------###########
Guardar Partida:
Oprimir la tecla 'G' en la ventana del juego. Se crea log de texto confirmando realización del guardado en el directorio de main.

Cargar Partida:
Seleccionar la opción 1 al inicio de la ejecución del juego.

###########--------- Instalación del Programa ---------###########
Requerimientos:
Simple DirectMedia Layer 3 (SDL3)

Instalacion de SDL3:
1. Descargar archivos comprimidos desde el repositorio oficial de github de la biblioteca: https://github.com/libsdl-org/SDL/releases y https://github.com/libsdl-org/SDL_image/releases.

2. Extraer archivos, revisar si la herramienta 'cmake' está instalada. Usar la instruccion: 'cmake --version'. De no tenerla instalada, instalarlo con 'sudo apt install cmake'.

3. Compilar, dentro de la carpeta del archivo extraido, las siguientes instrucciones:
'cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr/local'

4. Para revisar si SDL3 fue correctamente instalado, usar la instruccion 'pkg-config --modversion sdl3'.

Observacion:
Este proceso debe ser realizado para los dos archivos comprimidos adjuntos, pues uno es para instalar la libreria general de SDL3 y el segundo es necesario para procesar las imágenes correctamente.

Uso de Makefile:
'make' para compilar
'make clean' para eliminar archivos objeto
'make play' para ejecutar aplicación (equivalente a usar './plantas_vs_zombies')

Uso del ejecutable:
'./plantas_vs_zombies' para ejecutar aplicación
