#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo{
	void* dato;
	struct nodo* prox;
}nodo_t;


struct cola{
	nodo_t* prim;
	nodo_t* ult;
	size_t cant;
};


void cola_destruir(cola_t* cola, void (*destruir_dato)(void*)){
	while(!cola_esta_vacia(cola)){
		void* desencolado = cola_desencolar(cola);
		if(destruir_dato != NULL){
			destruir_dato(desencolado);
		}
	}
	free(cola);
}


void* cola_desencolar(cola_t* cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	void* dato_prim = cola->prim->dato;
	nodo_t* nodo = cola->prim; 
	cola->prim = cola->prim->prox;
	if (!cola->prim){
		cola->ult = NULL;
	}
	free(nodo);
	cola->cant--;
	return dato_prim;
}


void* cola_ver_primero(const cola_t* cola){
	return cola_esta_vacia(cola) ? NULL : cola->prim->dato;
}


bool cola_encolar(cola_t* cola, void* valor){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo){
		return false;
	}
	nuevo_nodo->dato = valor;
	nuevo_nodo->prox = NULL;
	if (cola_esta_vacia(cola)){
		cola->ult= nuevo_nodo;
		cola->prim = nuevo_nodo;

	}else{
		cola->ult->prox = nuevo_nodo;
		cola->ult = nuevo_nodo;
	}
	
	cola->cant++;
	return true;
}


bool cola_esta_vacia(const cola_t* cola){
	if (cola->prim == NULL && cola->ult == NULL){
		return true;
	}
	return false;
}


cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (!cola){
		return NULL;
	}
	cola->prim = NULL;
	cola->ult = NULL;

	return cola;
}

size_t cola_cantidad(cola_t* cola){
	return cola->cant;
}