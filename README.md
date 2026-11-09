# *Plantas vs Zombies*
***Fecha de Entrega: 18 de Noviembre, a las 23:59 horas.***  
***Fecha de Presentación: 23 de Noviembre - 4 de Diciembre.***  
## Organización del proyecto
### Lectura de la Entrada
**Entrada:** Archivo de Configuración & Archivo del Mapa  
**Salida:** Matriz del Juego *(Matriz_Entidades)*  
### Lógica del Juego  
**Entrada:** Matriz del Juego *(Matriz_Entidades)*  
**Salida:** Matriz Actualizada *(Matriz_Entidades & Matriz_Proyectiles)*
### Visualización del Mapa  
**Entrada:** Matriz Actualizada *(Matriz_Entidades & Matriz_Proyectiles)*  
**Salida:** Representación Visual del Juego 
### Nota
Tanto *Matriz_Entidades* como *Matriz_Proyectiles* son triple punteros de tipo int (int***). Deben estar implementadas usando memoría dinámica, mediante el uso de malloc().
## Paquetes de Mejoras
### Paquete 1: Almacenamiento & Cambios de Estados
*W.I.P.*
### Paquete 2: Mejora en la Jugabilidad
Nuevas plantas y nuevos zombies. Los proyectiles son una estructura separada que reaccionan a las casillas que atraviesan.
### Paquete 3: Mejoras en la Visualización
*W.I.P.*
## Plantas
### Girasol (1)
Planta generadora de soles. 
### Lanzaguisantes (2)
Planta defensiva, lanza arvejas/guisantes hacia los zombies en forma de proyectiles.
### Nuez (3)
Planta exclusivamente defensiva, tiene muchos puntos de vida.
### Lanzaguisantes de Hielo (4)
Planta defensiva, lanza arvejas/guisantes congelados, los cuales ralentizan a los zombies al impactar.
### Tronco de Fuego (5)
Planta defensiva, cuando una arveja/guisante la atraviesa, se prende en llamas, haciendo que tenga el doble de daño.
### Girasol Doble (6)
Planta generadora de soles, genera el doble de soles que el girasol regular.
## Zombies
### Zombie Común (-1)
Zombie regular, tiene la menor cantidad de vida de todos los zombies, pero es el más frecuente.
### Zombie con Bandera (-2)
Tiene la misma cantidad de vida que el zombie común, pero es más rápido.
### Zombie con Cono (-3)
Tiene más vida que el zombie común, resistiendo más impactos de proyectiles/arvejas/guisantes.
### Zombie con Balde (-4)
Tiene aún más vida que el zombie con cono, aunque es el zombie menos frecuente, es el que más impactos de arvejas resiste.

