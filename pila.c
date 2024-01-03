#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define CAPACIDAD_INICIAL 5


/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...

pila_t* pila_crear(void){
	
	pila_t* pila = malloc(sizeof(pila_t));
	
	if (pila == NULL){ 
		return NULL;
	}
	pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));
	if (pila->datos == NULL){
		free(pila);
		return NULL;
	}
	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INICIAL;

	return pila;
}

bool pila_redimensionar(pila_t* pila, size_t tamanio){
	if (tamanio < CAPACIDAD_INICIAL){
		tamanio = CAPACIDAD_INICIAL;
	}
	void** datos_nuevos = realloc(pila->datos, tamanio * sizeof(void*));
	if (datos_nuevos == NULL){
		return false;
	}
	pila->datos = datos_nuevos;
	pila->capacidad = tamanio;
	return true;
}

bool pila_esta_vacia(const pila_t* pila){
	return pila->cantidad == 0 ? true : false;
}

void* pila_ver_tope(const pila_t* pila){
	return pila_esta_vacia(pila) ? NULL : pila->datos[pila->cantidad -1];
}

void pila_destruir(pila_t* pila){
	free(pila->datos);
	free(pila);
}

void* pila_desapilar(pila_t* pila){
	if (pila_esta_vacia(pila)){
		return NULL;
	}
	void* tope = pila_ver_tope(pila);	
	if (pila->capacidad > CAPACIDAD_INICIAL && pila->cantidad <= pila->capacidad / 4){     
		pila_redimensionar(pila, pila->capacidad / 2);
	} 
	pila->cantidad--;
	return tope;
}

bool pila_apilar(pila_t* pila, void* valor){
	if (pila->cantidad >= pila->capacidad){
		if (!pila_redimensionar(pila, 2 * pila->capacidad)){
			return false;
		}
	}
	if (pila->datos == NULL){
		return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}
