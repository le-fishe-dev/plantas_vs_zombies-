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
'make play' para ejecutar aplicación

Uso del ejecutable:
'./plantas_vs_zombies' para ejecutar aplicación
