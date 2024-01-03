#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"
#include <string.h>

#define TAM_INICIAL 13
#define CONST_REDIMENSION 2
#define PRIMERA_POS 0


/***** DEFINICION DE STRUCT HEAP ****/

struct heap
{
	void** datos;
	size_t tam;
	size_t cant;
	cmp_func_t comparar;
};

/********* FUNCIONES AUXILIARES *********/

static bool heap_redimensionar(heap_t* heap, size_t tam_redimension);

static size_t pos_padre(size_t hijo);

static size_t pos_hijo_der(size_t padre);

static size_t pos_hijo_izq(size_t padre);

static void swap(void** arreglo, size_t padre, size_t hijo);

static size_t obtener_indice_maximo(void** arreglo, size_t padre, size_t hijo_der, size_t hijo_izq, cmp_func_t cmp, size_t tam);

static void upheap(void** arreglo, size_t indice_hijo, cmp_func_t cmp);

static void downheap(void** arreglo, size_t tam, size_t padre, cmp_func_t cmp);

static void heapify(void** arreglo, size_t tam, cmp_func_t cmp);


/*********	PRIMITIVAS HEAP *************/

heap_t* heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	heap->datos = malloc(TAM_INICIAL *  sizeof(void*));
	if(!heap->datos)
	{
		free(heap);
		return NULL;
	}
	heap->tam = TAM_INICIAL;
	heap->cant = 0;
	heap->comparar = cmp;

	return heap;
}


heap_t* heap_crear_arr(void* arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap){
		return NULL;
	}
	
	void** arr_aux = malloc(n * sizeof(void*));
	if(!arr_aux){
		free(heap);
		return NULL;
	}
	for (size_t i = 0; i < n; i++){
		arr_aux[i] = arreglo[i];
	}

	heapify(arr_aux, n, cmp);
	
	heap->datos = arr_aux;
	heap->cant = n;
	heap->tam = n;
	heap->comparar = cmp;
	
	return heap;
}

void* heap_desencolar(heap_t* heap){
	if(heap_esta_vacio(heap)){
		return NULL;
	}
	if (heap->tam > TAM_INICIAL && heap->cant <= (heap->tam / 4)){
		if(!heap_redimensionar(heap, heap->tam / CONST_REDIMENSION)){
			return NULL;
		}
	}
	void* elem = heap_ver_max(heap);

	swap(heap->datos, PRIMERA_POS, heap->cant - 1);
	heap->cant--;
	downheap(heap->datos, heap->cant, PRIMERA_POS, heap->comparar);
	return elem;
}


bool heap_encolar(heap_t* heap, void* elem){
	if (!elem || !heap->datos){
		return false;
	}
	if (heap->cant >= heap->tam){
		if(!heap_redimensionar(heap, CONST_REDIMENSION * heap->tam)){
			return false;
		}
	}
	heap->datos[heap->cant] = elem;
	upheap(heap->datos, heap->cant, heap->comparar);
	heap->cant++;
	return true;
}


void* heap_ver_max(const heap_t* heap){
	if (heap_esta_vacio(heap))
		return NULL;
	return heap->datos[0];	
}


bool heap_esta_vacio(const heap_t* heap){
	return heap->cant == 0;
}


size_t heap_cantidad(const heap_t* heap){
	return heap->cant;
}


void heap_destruir(heap_t* heap, void (*destruir_elemento)(void *e)){
	void* aux = NULL;
	while(!heap_esta_vacio(heap)){
		aux = heap_desencolar(heap);
		if(destruir_elemento != NULL)
			destruir_elemento(aux);
	}
	free(heap->datos);
	free(heap);
}


/******* DEFINICIONES DE FUNCIONES AUXILIARES **********/

static bool heap_redimensionar(heap_t* heap, size_t tam_redimension){
	if (tam_redimension < TAM_INICIAL){
		tam_redimension = TAM_INICIAL;
	}

	void** datos_nuevos = realloc(heap->datos, tam_redimension * sizeof(void*));
	if(!datos_nuevos){
		return false;
	}
	heap->datos = datos_nuevos;
	heap->tam = tam_redimension;
	return true;	
}


static size_t pos_padre(size_t hijo){
	return (hijo-1) / 2;
}


static size_t pos_hijo_der(size_t padre){
	return (2 * padre) + 2;
}


static size_t pos_hijo_izq(size_t padre){
	return (2 * padre )+ 1;
}


static void swap(void** arreglo, size_t padre, size_t hijo){
	void* padre_aux = arreglo[padre];
	arreglo[padre] = arreglo[hijo];
	arreglo[hijo] = padre_aux;
}


static size_t obtener_indice_maximo(void** arreglo, size_t padre, size_t hijo_der, size_t hijo_izq, cmp_func_t cmp, size_t tam){
	size_t max = 0;

	if(hijo_der >= tam){
		max = hijo_izq;
	}else{
		max = cmp(arreglo[hijo_izq], arreglo[hijo_der]) > 0 ? hijo_izq : hijo_der;
	}

	return cmp(arreglo[max], arreglo[padre]) > 0 ? max : padre;

}


static void downheap(void** arreglo, size_t tam, size_t padre, cmp_func_t cmp){
	if (padre >= tam) 
		return;
	size_t hijo_der = pos_hijo_der(padre);
	size_t hijo_izq = pos_hijo_izq(padre);
	if(hijo_izq >= tam){
		return;
	}
	size_t maximo = obtener_indice_maximo(arreglo, padre, hijo_der, hijo_izq, cmp, tam);
	if(maximo != padre){
		swap(arreglo, maximo, padre);
		downheap(arreglo, tam, maximo, cmp);
	}
	
}


static void upheap(void** arreglo, size_t indice_hijo, cmp_func_t cmp){
	if (indice_hijo == 0){
		return;
	}
	size_t indice_padre = pos_padre(indice_hijo);
	if (cmp(arreglo[indice_hijo], arreglo[indice_padre]) > 0)
	{
		swap(arreglo, indice_hijo, indice_padre);
		upheap(arreglo, indice_padre, cmp);
	}
}


static void heapify(void** arreglo, size_t tam, cmp_func_t cmp){
	for (size_t i = tam; i > 0; i--)
	{
		downheap(arreglo, tam, i-1, cmp);
	}
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos, cant, cmp);
	for(size_t i = 0; i < cant; i++){
		swap(elementos, PRIMERA_POS, cant-i-1);
		downheap(elementos, cant-i-1, PRIMERA_POS, cmp);
	}
}
