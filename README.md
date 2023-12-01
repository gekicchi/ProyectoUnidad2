# The Clash Of The Guardians
## Problema
La creacion de una aplicacion jugable llamada "The Guardians Battles" con las funcionalidades minimas de:
* Lectura de guardianes y ciudades de un archivo externo
* Ver lista de Guardianes
* Ver informacion de Guardian seleccionado
* Ver y Construir caminos entre ciudades
* Combate entre 2 Guardianes y Reordenamiento de Ranking
## Solución
### 3 estructuras:
* Guardián; con nombre, Nivel, Ciudad de Origen, Mestro
* RankedGuardian; con un puntero a su Guardian, puntero a Guardian de menor y mayor nivel
* GeneralGuardian; con un puntero a su Guardian, lista de Guardianes aprendices
### 3 clases:
* Ranking; Arbol Binario con funcionalidades de Insertar, Buscar por Nivel, Imprimir
* Hierarchy; Arbol General con funcionalidades de Insertar, Buscar por Nombre, Conseguir un Guardian, Imprimir
* Map; Grafo con funcionalides de Añadir Camino, Comprobar Adyacencia, Buscar Camino, Imprimir
