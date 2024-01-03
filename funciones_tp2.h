#ifndef __FUNCION__H_
#define __FUNCION__H_ 

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef int (*funct_t) (const void *a, const void *b);

typedef enum {TODOS_LOS_ELEM, SIN_MINIMO, SIN_MAXIMO, CON_MIN_Y_MAX} extremos_t;

typedef enum {LIBRE, OCUPADO} estado_t;

/************* DECLARACION DE ESTRUCTURAS *****************/


/* Estructura clinica donde se tiene todos los datos 
 * de pacientes, doctores y especialidades
 */
typedef struct clinica clinica_t;


/* Estructura paciente donde guarda la informacion de este
 * (nombre y antiguedad) */
typedef struct paciente paciente_t;


/* Estructura doctor, guarda su nombre, especialidad
 * cantidad de pacientes atendidos y su estado donde puede 
 * estar ocupado o libre 
 */
typedef struct doctor doctor_t;


/* Estructura cola paciente, contiene los dos colas una de urgentes
 * y otra de regulares, donde almancena a los pacientes 
 */
typedef struct cola_pacientes cola_pacientes_t;


/********* DECLARACIONES DE FUNCIONES PARA TP2 ****************/


/* Inicializa la clinica recibiendo como parametros dos arboles
 * con los datos de los doctores y pacientes.
 * Pre: Los arboles fueron creados.
 * Post: devuelve la clinica inicializada lista para operar. 
 */
clinica_t* init( abb_t* arbol_doctores, abb_t* arbol_pacientes);


/* Crea la cola de pacientes tanto para los pacientes urgentes 
 * como para los pacientes regulares, recibe una funcion de comparacion
 * con la cual define la prioridad al momento de atender.  
 */
cola_pacientes_t* cola_pacientes_crear(funct_t cmp);


/* Crea el doctor en la clinica, recibe el nombre y su especialidad
 */
doctor_t* doctor_crear(const char* nombre, char* esp);


/* Crea el paciente en la clinica, recibe el nombre y la antiguedad
 * dentro de la clinica
 */
paciente_t* paciente_crear(const char* nombre, size_t anio_i);


/* Valida que la cantidad de parametros ingresados sea valida
 */
bool cant_params_check(char* cmd, size_t cant_params, const char** parametros);


/* Define cual comando de la clinica fue introducido
 * redirigiendo a este a su funcion la cual lo procesara
 */
void cmd_load(const char* comando, const char** parametros, clinica_t* clinica);


/* Elimina el caracter de fin de linea de una 
 * cadena de texto pasada por parametro
 */
void eliminar_fin_linea(char* linea);


/* Divide los parametros ingresados para luego 
 * llamar a la funcion cmd_load que los procesara
 * junto con la clinica pasada por parametro.
 * Pre: la clinica fue inicializada
 * Post: parsea la entra, lista para ser procesada
 * por las funciones de comandos.
 */ 
bool procesar_entrada(clinica_t* clinica);


/* Pide turno para una determinada especialidad ingresada por paramentro
 * y la encola en la lista de espera de esta.
 * Pre: la clinica fue inicializada.
 * Pre: se encolo en la lista de espera el paciente.
 */
void pedir_turno(const char** parametros, clinica_t* clinica);


/* Imprime por pantalla la cantidad de doctores en el sistema y el informe 
 * de todos los doctores de la clinica junto con su nombre, especialidad, y cantidad 
 * de pacientes que atendieron cada uno
 * Pre: la clinica fue inicializada
 */
void informe(const char** parametros, clinica_t* clinica);


/*Libera la memoria utilizada
*/
void clinica_destruir(clinica_t* clinica);


/* Atiende al siguiente paciente, dependiendo 
 * si es urgente o regular
 * Pre: la clinica fue inicializada 
 * Post: El paciente es atendido y es desencolado de la lista de espera
 */
void atender_siguiente(const char** parametros, clinica_t* clinica);


/* funcion wrapper de struct de paciente */
void* paciente_wrapper(char** params);


/* funcion wrapper de struct de doctor */
void* doctor_wrapper(char** params);


/* Destruye la estructura interna del doctor liberando 
 * toda la memoria
 */
void doctor_destruir(doctor_t* d);


/* Destruye la estructura interna del doctor liberando 
 * toda la memoria
 */
void paciente_destruir(paciente_t* p);


/*funcion wrapper de destruccion de doctor */
void doctor_destruir_wrapper(void*);


/*funcion wrapper de destruccion de paciente */
void paciente_destruir_wrapper(void*);


/* Destruye la cola de espera de los pacientes */
void cola_pacientes_destruir(cola_pacientes_t* pacientes);


/*funcion wrapper de destruccion de cola de pacientes */
void cola_pacientes_destruir_wrapper(void* q);


/* Devuelve un entero dependiendo la cantidad de extremos 
 * que es ingresado por entrada estandar :

 * devuelve 0 si no tiene ningun extremos
 * devuelve 1 si no tiene minimo
 * devuelve 2 si no tiene maximo
 * devuelve 3 si tiene tanto minimo como maximo
 */
int obtener_num_extremos(const char* min, const char* max);


/* llama a la primitiva de abb (abb_iter_in_por_rangos) 
 * dependiendo de la cantidad de extremos que se pasa por parametro
 */
void iteracion_por_rangos(abb_t* abb, bool (*visitar) (const char*, void*, void*), void* extra, int cant_extremos, const char* min, const char* max);


/*funcion que utiliza el iterador interno de abb para visitar cada 
 * elemento de este. realizado un informe de los doctores
 */
bool func_visitar_informe(const char* clave, void* valor, void* extra);


/* Compara los pacientes por medio de su antiguedad */
int comparar_pacientes(const void* a, const void* b);

#endif