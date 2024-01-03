#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "lista.h"
#include "hash.h"
#include <stdbool.h>

#define BORRAR true
#define NO_BORRAR false


#define TAM_INICIAL 41
#define VACIO 0
#define FACTOR_CARGA_MAXIMO 2.3
#define INDICE_REDIMENSION 2

#define SEED 4

/*********** ESTRUCTURAS *************/

struct hash_iter {
    const hash_t* hash;
    size_t recorridos;
    size_t posicion;
    lista_iter_t* lista_iter;
};

struct hash {
	lista_t** tabla;
	size_t tam;
	size_t cant;
	hash_destruir_dato_t destruir;
};


typedef struct campos {
	char* clave;
	void* dato;
} campos_t;

/*************************************/

/********** FUNCIONES AUXILIARES IMPLEMENTACIÓN ITERADOR HASH **********/

void buscar_posicion_lista_no_vacia(hash_iter_t* iter);

/***********************************************************************/

/********** FUNCIONES AUXILIARES IMPLEMENTACIÓN DE HASH **********/

bool hash_redimension(hash_t* hash);

lista_t** crear_tabla(void);

bool tabla_llenar(lista_t** tabla, size_t min, size_t max);

float factor_carga(hash_t* hash);

campos_t* buscar_clave_en_lista(lista_t* lista, const char* clave, bool borrar);

campos_t* buscar_clave_en_hash(const hash_t *hash, const char* clave, bool borrar);

bool reubicar_claves(hash_t* hash, size_t tam_anterior, lista_t** tabla);

/****************************************************************/


/*********** MURMUR3 32bit HASH ***************/

static size_t murmur_32_scramble(size_t k);

size_t murmur3_32(const char* key, size_t len, unsigned int seed);

/**************************************************/


/******************** PRIMITIVAS DE TABLA DE HASH **********************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;
	hash->tabla = crear_tabla();
	if(!hash->tabla)
		return NULL;
	hash->tam = TAM_INICIAL;
	hash->cant = VACIO;
	hash->destruir = destruir_dato;
	return hash;
}

void hash_destruir(hash_t *hash){
	for(size_t i = 0; i < hash->tam; i++){
		while(!lista_esta_vacia(hash->tabla[i])){
			campos_t* actual = lista_borrar_primero(hash->tabla[i]);
			if(hash->destruir != NULL)
				hash->destruir(actual->dato);
			free(actual->clave);
			free(actual);
		}
		lista_destruir(hash->tabla[i], NULL);
	}
	free(hash->tabla);
	free(hash);
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato){
	char* copia_clave = strdup(clave);
	if(!copia_clave)
		return false;
	if(factor_carga(hash) > FACTOR_CARGA_MAXIMO)
		if(!hash_redimension(hash))
			return false;
	campos_t* campo = malloc(sizeof(campos_t));
	campo->clave = copia_clave;
	campo->dato = dato;
	size_t indice = murmur3_32(clave, strlen(clave), SEED) % hash->tam;
	if(lista_esta_vacia(hash->tabla[indice])){
		lista_insertar_primero(hash->tabla[indice], campo);
		hash->cant++;
		return true;
	}
	campos_t* clave_encontrada = buscar_clave_en_lista(hash->tabla[indice], clave, NO_BORRAR);
	if (clave_encontrada){
		if(hash->destruir != NULL)
			hash->destruir(clave_encontrada->dato);
		clave_encontrada->dato = dato;
		free(campo->clave);
		free(campo);
	}else{
		lista_insertar_primero(hash->tabla[indice], campo);
		hash->cant++;
	}
	return true;
}

void* hash_obtener(const hash_t* hash, const char* clave){
	campos_t* elemento = buscar_clave_en_hash(hash, clave, NO_BORRAR);
	if(!elemento)
		return NULL;
	return elemento->dato;
}

size_t hash_cantidad(const hash_t* hash){
	return hash->cant;
}

bool hash_pertenece(const hash_t* hash, const char* clave){
	campos_t *pertenece = buscar_clave_en_hash(hash, clave, NO_BORRAR);
	if(pertenece)
		return true;
	return false;
}

void* hash_borrar(hash_t *hash, const char* clave){
	campos_t *campos = buscar_clave_en_hash(hash, clave, BORRAR);
	if(!campos)
		return NULL;
	void* dato = campos->dato;
	free(campos->clave);
	free(campos);
	hash->cant--;
	return dato;

}

/********************** PRIMITIVAS HASH ITERADOR *****************/
 
hash_iter_t* hash_iter_crear(const hash_t* hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if(!iter)
        return NULL;
    iter->recorridos = 0;
    iter->posicion = 0;
    iter->hash = hash;
    if (iter->hash->cant == VACIO){
        iter->lista_iter = NULL;
    } else{
        buscar_posicion_lista_no_vacia(iter);
        iter->lista_iter = lista_iter_crear(iter->hash->tabla[iter->posicion]);
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter){
    if (hash_iter_al_final(iter)){
        return false;
    } else if (lista_iter_avanzar(iter->lista_iter)){
        iter->recorridos++;
    }
    if (lista_iter_al_final(iter->lista_iter) && !hash_iter_al_final(iter)){
        lista_iter_destruir(iter->lista_iter);
        iter->posicion++;
        buscar_posicion_lista_no_vacia(iter);
        iter->lista_iter = lista_iter_crear(iter->hash->tabla[iter->posicion]);
    }
    return true;
}
 
const char* hash_iter_ver_actual(const hash_iter_t* iter){
	if(hash_iter_al_final(iter))
		return NULL;
	if(!iter->hash->cant)
		return NULL;
	campos_t* clave_actual = (campos_t*)lista_iter_ver_actual(iter->lista_iter);
	return clave_actual->clave;
}
 

bool hash_iter_al_final(const hash_iter_t* iter){
    return iter->recorridos == iter->hash->cant;
}

void hash_iter_destruir(hash_iter_t* iter){
	lista_iter_destruir(iter->lista_iter);
	free(iter);
}

/********************** FUNCION DE HASH *************************/

/*Función sacada de Wikipedia. En el foro Software Engineering preguntaron cuál es
la función de hash mas rápida y más eficiente. Le respondieron con varias pruebas
de tiempo y la más rapida resultó ser Murmur. Están también los resultados de otros algoritmos
como FNV-1, SuperFastHash, SDBM...
https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed
*/

static inline size_t murmur_32_scramble(size_t k) {
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}
size_t murmur3_32(const char* key, size_t len, unsigned int seed){
	size_t h = seed;
    unsigned int k = 0;
    for (size_t i = len >> 2; i; i--) {
        memcpy(&k, key, sizeof(unsigned int));
        key += sizeof(unsigned int);
        h ^= murmur_32_scramble(k);
        h = (h << 13) | (h >> 19);
        h = h * 5 + 0xe6546b64;
    }
    k = 0;
    for (size_t i = len & 3; i; i--) {
        k <<= 8;
        k |=(unsigned char) key[i - 1];
    }
    h ^= murmur_32_scramble(k);
	h ^= len;
	h ^= h >> 16;
	h *= 0x85ebca6b;
	h ^= h >> 13;
	h *= 0xc2b2ae35;
	h ^= h >> 16;
	return h;
}

/************************ FUNCIONES AUXILIARES *****************************/

bool hash_redimension(hash_t* hash){
	size_t tam_anterior = hash->tam;
	hash->tam = tam_anterior * INDICE_REDIMENSION;
	lista_t** nueva_tabla = calloc(hash->tam, sizeof(lista_t*));
	if(!nueva_tabla){
		hash->tam = tam_anterior;
		return false;
	}
	lista_t** tabla_anterior = hash->tabla;
	hash->tabla = nueva_tabla;
	tabla_llenar(hash->tabla, 0, hash->tam);
	if(!reubicar_claves(hash, tam_anterior, tabla_anterior))
		return false;
	return true;
}

bool tabla_llenar(lista_t** tabla, size_t min, size_t max){
	for(size_t i = min; i < max; i++){
		tabla[i] = lista_crear();
		if (tabla[i] == NULL)
			return false;
	}
	return true;
}

float factor_carga(hash_t* hash){
	float a = (float)(hash->cant/hash->tam);
	return a;
}

void buscar_posicion_lista_no_vacia(hash_iter_t* iter){
    while(lista_esta_vacia(iter->hash->tabla[iter->posicion]) && !hash_iter_al_final(iter))
        iter->posicion++;
}

campos_t* buscar_clave_en_lista(lista_t* lista, const char* clave, bool borrar){
	campos_t* clave_dato = NULL;
	lista_iter_t* iter = lista_iter_crear(lista);
	if(!iter)
		return NULL;
	while(!lista_iter_al_final(iter)){
		campos_t* actual = (campos_t*)lista_iter_ver_actual(iter);
		if (!strcmp(actual->clave, clave)){
			clave_dato = actual;
			if(borrar){
				actual = lista_iter_borrar(iter);
				lista_iter_destruir(iter);
				return actual;
			}
			break;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return clave_dato;
}

lista_t** crear_tabla(void){
	lista_t** tabla = calloc(TAM_INICIAL, sizeof(lista_t*));
	if(!tabla)
		return NULL;
	if(!tabla_llenar(tabla, 0, TAM_INICIAL))
		return NULL;
	return tabla;
}

campos_t* buscar_clave_en_hash(const hash_t *hash, const char* clave, bool borrar){
	size_t indice = murmur3_32(clave, strlen(clave), SEED)%hash->tam;
	if(lista_esta_vacia(hash->tabla[indice]))
		return NULL;
	campos_t* campos = buscar_clave_en_lista(hash->tabla[indice], clave, borrar);
	if(campos)
		return campos;
	return NULL;
}

bool reubicar_claves(hash_t* hash, size_t tam_anterior, lista_t** tabla){
	hash->cant = 0;
	for(size_t i = 0; i<tam_anterior; i++){
		while(!lista_esta_vacia(tabla[i])){
			campos_t* actual = lista_borrar_primero(tabla[i]);
			if(!hash_guardar(hash, actual->clave, actual->dato))
				return false;
			free(actual->clave);
			free(actual);
		}
		lista_destruir(tabla[i], NULL);
	}
	free(tabla);
	return true;
}
