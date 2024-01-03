#include "lista.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef struct nodo {
	void* dato;
	struct nodo* prox;
}nodo_t;


struct lista {
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};


/* primitivas lista */


lista_t* lista_crear(){
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista){
		return NULL;
	}
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;

	return lista;
}


bool lista_esta_vacia(const lista_t* lista){
	return lista->largo == 0;
}


bool lista_insertar_primero(lista_t* lista, void* dato){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo){
		return false;
	}
	nuevo_nodo->dato = dato;
	nuevo_nodo->prox = NULL;
	if (lista_esta_vacia(lista)){
		lista->ult = nuevo_nodo;
	}else
		nuevo_nodo->prox = lista->prim;
	lista->prim = nuevo_nodo;
	lista->largo++;
	return true;

}


bool lista_insertar_ultimo(lista_t* lista, void* dato){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo){
		return false;
	}
	nuevo_nodo->dato = dato;
	nuevo_nodo->prox = NULL;
	if (lista_esta_vacia(lista)){
		lista->prim = nuevo_nodo;
	}else
		lista->ult->prox = nuevo_nodo;
	
	lista->ult = nuevo_nodo;
	lista->largo++;
	return true;
}


void* lista_borrar_primero(lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	void* dato = lista->prim->dato;
	nodo_t* borrar = lista->prim;
	lista->prim = lista->prim->prox;
	if (!lista->prim){
		lista->ult = NULL;
	}

	free(borrar);
	lista->largo--;
	
	return dato;
}


void* lista_ver_primero(const lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->prim->dato;
}


void* lista_ver_ultimo(const lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->ult->dato;
}


size_t lista_largo(const lista_t* lista){
	return lista->largo;
}


void lista_destruir(lista_t* lista, void (*destruir_dato)(void*)){
	while(!lista_esta_vacia(lista)){
		void* borrar = lista_borrar_primero(lista);
		if(destruir_dato){
			destruir_dato(borrar);
		}
	}
	free(lista);
}


/* primitivas iterador externo */

struct lista_iter {
	nodo_t* act;
	nodo_t* ant;
	lista_t* lista;

};

lista_iter_t* lista_iter_crear(lista_t* lista){

	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter){
		return NULL;
	}
	iter->act = lista->prim;
	iter->ant = NULL;
	iter->lista = lista;

	return iter;
}


bool lista_iter_avanzar(lista_iter_t* iter){
	if (lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)){ 
		return false;
	}
	iter->ant = iter->act;
	iter->act = iter->act->prox;
	return true;
}


void* lista_iter_ver_actual(const lista_iter_t* iter){
	if(lista_iter_al_final(iter)){
		return NULL;
	}
	return iter->act->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter){
	return iter->act == NULL;
}

void lista_iter_destruir(lista_iter_t* iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato){
	nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo){
		return false;
	}
	nuevo_nodo->dato = dato;
	nuevo_nodo->prox = NULL;
	
	if (iter->act == iter->lista->prim){
		nuevo_nodo->prox = iter->act;
		iter->lista->prim = nuevo_nodo;
	}
	
	if (iter->ant == iter->lista->ult){
		iter->lista->ult = nuevo_nodo;
	}
	
	if (iter->ant){
		nuevo_nodo->prox = iter->act;
		iter->ant->prox = nuevo_nodo;
	}
	iter->act = nuevo_nodo;
	iter->lista->largo++;

	return true;
}

void* lista_iter_borrar(lista_iter_t* iter){
	if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)){
		return NULL;
	}
	void* dato = iter->act->dato;
	nodo_t* nodo_borrar = iter->act;
	if (iter->act == iter->lista->prim){
		iter->lista->prim = iter->act->prox;
	}
	if (iter->act == iter->lista->ult){
		iter->lista->ult = iter->ant;
	}
	if (iter->ant){
		iter->ant->prox = iter->act->prox;
	}
	
	iter->act = iter->act->prox;
	free(nodo_borrar);
	iter->lista->largo--;

	return dato;
}


/* primitiva iterador interno */

void lista_iterar(lista_t* lista, bool (*visitar)(void* dato, void* extra), void* extra){
	nodo_t* act = lista->prim;
	/* entra siempre al la primera iteracion del while */
	bool entrar_a_ciclo = true;
	while (entrar_a_ciclo && act){
		entrar_a_ciclo = visitar(act->dato, extra);
		act = act->prox;
	}
}
