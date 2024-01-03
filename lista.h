#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include <stdbool.h>

/*definicion de estructuras */ 

struct lista;
typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;


/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/


//crea una lista
//Post: devuelve una nueva lista vacia.
lista_t *lista_crear(void);

//devulve verdadero si la lista no contiene elementos,false en caso contrario
//Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

//agrega un nuevo elemento a la lista, devuelve false en caso de error.
//Pre: la lista fue creada.
//Post: se agrego un nuevo elemento a la lista, el elemento se encuentra al principio
//de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

//agrega un nuevo elemento a la lista, devuelve false en caso de error.
//Pre: la lista fue creada.
//Post: se agrego un nuevo elemento a la lista, el elemento se encuentra al final
//de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//saca el primer elemento de la lista. Si la lista tiene elementos, se quita el primero
//de la lista, y devuelve su valor, si esta vacia devuelve NULL.
//Pre: la lista fue creada.
//Post: se devolvio el primer elemento de la lista, la lista tiene un elemento nuevo
//si la lista no estaba vacia.
void* lista_borrar_primero(lista_t *lista);

//obtiene el valor del primer elemento de la lista, si tiene elementos
// si esta vacia devuelve NULL.
//Pre: la lista fue creada.
//Post: se devolvio el valor del primer elemento, cuando la lista no esta vacia
//en caso contrario NULL.
void* lista_ver_primero(const lista_t *lista);

//obtiene el valor del ultimo elemento de la lista, si tiene elementos.
// si esta vacia devuelve NULL.
//Pre: la lista fue creada.
//Post: se devolvio el valor del ultimo elemento, cuando la lista no esta vacia
//en caso contrario NULL.
void* lista_ver_ultimo(const lista_t* lista);

//obtiene el largo de la lista.
//Pre: la lista fue creada.
//Post: devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));


/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

//recorre la lista por sus elementos donde la funcion visitar es ejecutada,
//puede recibir un parametro opcional llamado extra.
//Pre: la lista fue creada.
//Post: la funcion visitar fue aplicada a los elementos de la lista.
void lista_iterar(lista_t* lista, bool (*visitar)(void* dato, void* extra), void* extra);



/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/


//crear un iterador.
//Post: devulve un iterador apuntando al primer elemento
//de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

//avanza al proximo elemento de la lista, si el iterador 
//se encuentra al final del lista o la lista no tiene 
//elementos devuelve false.
//Pre: la lista fue creada.
//Post: el iterador avanzo al proximo elemento de la lista
bool lista_iter_avanzar(lista_iter_t *iter);

//obtiene el valor donde actualmente se encuentra el iterador.
//Pre: la lista fue creada.
//Post: devulve el valor del actual, si la lista esta vacia
//devulve NULL;
void *lista_iter_ver_actual(const lista_iter_t *iter);

//devulve verdadero si el iterador se encuentra al final 
//en caso contrario devulve false.
//Pre: la lista fue creada.
bool lista_iter_al_final(const lista_iter_t *iter);

//destruye el iterador 
//Pre: la lista fue destruida
//Post: se eliminaron todos los elementos de la lista
void lista_iter_destruir(lista_iter_t *iter);

//inserta un nuevo elemento a la lista donde actualmente se 
//encuentra el iterador, en caso de error devulve false
//Pre: la lista fue creada.
//Post: el elemento fue insertado en la posicion actual del
//iterador, ahora la lista posee un elemento mas.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//borra un elemento de la lista donde se encuentra el iterador 
//y se devuelve su valor.Si la lista esta vacia o el iterador 
//se encuntra al final retorna NULL;
//Pre: la lista fue creada
//Post: el elemento fue retirado de la lista, devolviendo
//el valor que habia en el.La lista ahora posee un elemento menos
void *lista_iter_borrar(lista_iter_t *iter);


#endif 