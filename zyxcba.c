#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csv.h"

#include "mensajes.h"
#include "hash.h"
#include "abb.h"
#include "funciones_tp2.h"
#include "strutil.h"



int verificar_arch(abb_t* abb1, abb_t* abb2, char** argv){
	bool ok = true;
	if (!abb1){
		printf(ENOENT_ARCHIVO, argv[1]);
		ok = false;
	}
	if (!abb2){
		printf(ENOENT_ARCHIVO, argv[2]);
		ok = false;
	}
	return !ok ? 1 : 0;
}


int main(int argc, char** argv){
	if(argc != 3){
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}
	abb_t* arbol_doctores = csv_crear_abb(argv[1], doctor_wrapper, doctor_destruir_wrapper);
	abb_t* arbol_pacientes = csv_crear_abb(argv[2], paciente_wrapper, paciente_destruir_wrapper);

	int indice_corte = verificar_arch(arbol_doctores, arbol_pacientes, argv);
	if (indice_corte == 1) {
		abb_destruir(arbol_doctores);
		abb_destruir(arbol_pacientes);
		return indice_corte;
	}
	clinica_t* clinica = init(arbol_doctores, arbol_pacientes);
	bool seguir = true;
	while(seguir){
		seguir = procesar_entrada(clinica);
	}
	clinica_destruir(clinica);
}