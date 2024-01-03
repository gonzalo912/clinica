#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#define SEPARADOR ','

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}


abb_t* csv_crear_abb(const char* ruta_csv, void* (*creador) (char**), void (*destruir_dato) (void*)) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) 
		return NULL;
	abb_t* abb = abb_crear(strcmp, destruir_dato);
	if (!abb) {
		fclose(archivo);
		return NULL;
	}
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		void* indv = creador(campos);
		if(!indv){
			free(linea);
			free_strv(campos);
			abb_destruir(abb);
			fclose(archivo);
			return NULL;
		}
		abb_guardar(abb, campos[0],  indv);
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return abb;
}