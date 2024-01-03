#ifndef __CSV__H_
#define __CSV__H_

#include "abb.h"
#include "strutil.h"


/**
Haciendo uso de strutil (split) lee un archivo csv y, línea a línea,
va llamando al creador que se pasa por parámetro. Dicho creador se invoca
con la línea separada por split, sin fines de línea ni ninguna otra consideración,
y con el puntero extra que se pasa por parámetro.
Importante: la función creador no debe guardar las referencias a las cadenas
dentro de arreglo de cadenas pasado por parámetros (hacer copias en caso de ser 
necesario); luego de invocarse el creador se invoca a free_strv.

Se devuelve un abb con todos los elementos construidos. NULL en caso que el archivo
csv (indicado por la ruta pasada por parámetro) no exista. 
**/
abb_t* csv_crear_abb(const char* ruta_csv, void* (*creador) (char**), void (*destruir_dato) (void*));


#endif
