#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/***** definicion de estructuras ******/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;


/****funcion de comparacion *****/

typedef int (*abb_comparar_clave_t) (const char *, const char *);


/****funcion de destruccion *****/

typedef void (*abb_destruir_dato_t) (void *);



/***** primitivas del abb ******/

/* Crea el abb 
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra 
 * en el arbol se reemplaza el dato. Si no puede guardar devulve false.
 * Pre: El abb fue creado
 * Post: Se almaceno en el abb el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento en el abb y devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: El abb fue creado
 * Post : El elemento fue borrado de la estructura, en 
 * el caso de que estuviera guardado
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento en el abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: El abb fue creado.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si la clave pertenece o no al abb
 * Pre: La estructura del abb fue inicializada.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos en el abb
 * Pre: El abb fue creado
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura del abb liberando toda la memoria perdida
 * y llama a la funcion visitar en cada par (clave, dato).
 * Pre: La estructura abb fue creado
 * Post: La estructura abb fue destruida.
 */
void abb_destruir(abb_t *arbol);


/* Devuelve la mayor de la claves dentro del abb
 */ 
char* devolver_mayor_clave(const abb_t* arbol);


/* Devuelve la menor de la claves dentro del abb
 */ 
char* devolver_menor_clave(const abb_t* arbol);



/****** primitiva de iterador interno *********/


/* Recorre el abb por sus elemento donde la funcion visitar es ejecutada
 * puede recibir un parametro adicional llamado extra.
 * Pre: El abb fue creado
 * Post: La funcion visitar fue aplicada a los elementos de abb.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);



/*****primitivas del iterador externo ******/



/* Crea un iterador
 */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza al proximo elemento del abb, si el iterador
 * se encuentra al final devuelve false, en caso contrario true.
 * Pre: El abb fue creado.
 * Post: El iterador avanzo al proximo elemento del abb.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Obtiene la clave donde actualmente se encuentra el iterador
 * Pre: La estructura abb fue creada.
 * Post: Devuelve la clave donde esta el iterador
 */ 
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Devuelve true si el iterador se encuentra al final
 * en caso contrario devuelve false.
 * Pre: La estructura del abb fue inicializada.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* destruye el iterador iterador y toda su estructura interana
 */
void abb_iter_in_destruir(abb_iter_t* iter);


/******primitiva iterador intero por rangos ****/

void abb_in_order_por_rangos(abb_t* abb, bool (*visitar) (const char*, void*, void*), void* extra, const char* min, const char* max);



#endif	