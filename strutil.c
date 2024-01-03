#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

/* SE PUEDE USAR PARA IMPLEMENTAR CUALQUIER FUNCION : strcpy/strncpy, strlen, strdup/strndup, snprintf*/

char* substr(const char* str, size_t n){
	
	size_t largo_substr = 0;
	if (n > strlen(str)){
		largo_substr = strlen(str);
	}else{
		largo_substr = n;
	}
	
	char* devolver = calloc(largo_substr + 1 , sizeof(char));
	if(!devolver){
		return NULL;
	}
	for (size_t i = 0; i < largo_substr; i++){
		devolver[i] = str[i];
	}

	return devolver;
}


size_t cantidad_apariciones_sep(const char* str, char sep){
	
	size_t apariciones_sep = 0;
	for (size_t i = 0; i < strlen(str); i++){
		if (str[i] == sep){
			apariciones_sep++;
		}
	}
	apariciones_sep++;
	return apariciones_sep;
}


char** split(const char* str, char sep){
	
	size_t apariciones_sep = cantidad_apariciones_sep(str, sep);
	
	char** spliteado = calloc(apariciones_sep + 1, sizeof(char*));
	if(!spliteado){
		return NULL;
	}

	size_t posicion = 0;
	size_t inicio = 0;
	for (size_t j = 0; j <= strlen(str); j++){
		if (str[j] == sep || str[j] == '\0'){
			spliteado[posicion] = substr(str + inicio, j - inicio);
			inicio = j+1;
			posicion++;
		}
	}
	return spliteado;
}


char* join(char** strv, char sep){
	
	if (strv[0] == NULL){
		char* devolver = malloc(sizeof(char));
		devolver[0] = '\0'; 
		return devolver;
	}

	size_t cant_caracteres = 0;
	size_t cant_separadores = 0;
	while(strv[cant_separadores]){
		cant_caracteres += strlen(strv[cant_separadores]);
		cant_separadores++;
	}

	size_t largo_str_devolver = cant_caracteres + cant_separadores;
	char* cadena_a_devolver = calloc(largo_str_devolver, sizeof(char)); 
	if (!cadena_a_devolver){
		return NULL;
	}
	size_t inicio = 0;
	for (size_t i = 0; i < largo_str_devolver; i++){
		strcpy(cadena_a_devolver + inicio, strv[i]);

		if(strv[i + 1] == NULL){
			cadena_a_devolver[largo_str_devolver - 1] = '\0'; 
			return cadena_a_devolver;
		}
		
		size_t largo = strlen(strv[i]);
		if (sep != '\0'){
			cadena_a_devolver[inicio + largo] = sep;
			inicio = inicio + largo + 1;
		}else{
			inicio = inicio + largo;
		}
	}
	return cadena_a_devolver;
}


void free_strv(char* strv[]){
	
	int i = 0;
	while (strv[i]){
		free(strv[i]);
		i++;
	}
	free(strv);
}

