#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PADRE(hijo) (((hijo) - 1) / 2)
#define HIJO_IZQ(padre) (((padre) * 2) + 1)
#define HIJO_DER(padre) (((padre) * 2) + 2)

#define TAMANIO_INICIAL 100
#define FACTOR_REDIMENSION_AUMENTAR 2
#define FACTOR_REDIMENSION_REDUCIR 2
#define FACTOR_CHECK_REDUCIR 2

struct heap{
	void** vector;
	cmp_func_t cmp;
	size_t tam;
	size_t cantidad;
};

/* ******************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

bool heap_redimensionar(heap_t *heap, size_t tam_nuevo){

	void **datos_nuevos = realloc(heap->vector, sizeof(void*) * tam_nuevo);
	if(datos_nuevos == NULL){
		return false;
	}
	heap->vector = datos_nuevos;
	heap->tam = tam_nuevo;
	return true;
}

/*Recibe una posición con hijo válidos y devuelve la posición del hijo mayor*/

size_t calcular_pos_hijo_mayor(void* arr[],size_t pos_padre, size_t tope, cmp_func_t cmp){

	size_t pos_hijo_mayor = HIJO_IZQ(pos_padre);

	if (HIJO_DER(pos_padre) < tope) { //si hay dos hijos
		if (cmp(arr[HIJO_IZQ(pos_padre)] , arr[HIJO_DER(pos_padre)]) < 0){
			pos_hijo_mayor = HIJO_DER(pos_padre);
		}
	}
	return pos_hijo_mayor;
}

/*Recibe un vector y dos posiciones, reliza un swap entre los elementos de estas posiciones*/

void swap(void* arr[],size_t pos_padre, size_t pos_hijo_mayor) {

	void * aux = arr[pos_padre];
	arr[pos_padre] = arr[pos_hijo_mayor];
	arr[pos_hijo_mayor] = aux;
}

void _downheap (void* arr[],size_t pos_actual, size_t tope, cmp_func_t cmp){

	if(HIJO_IZQ(pos_actual) >= tope) return;

	size_t pos_hijo_mayor = calcular_pos_hijo_mayor(arr, pos_actual, tope, cmp);

	if (cmp(arr[pos_actual], arr[pos_hijo_mayor]) >= 0) return;

	swap(arr, pos_actual, pos_hijo_mayor);
  _downheap(arr, pos_hijo_mayor, tope, cmp);

}

/*En caso de ser necesario reubica al elemento de la posición indicada en su lugar correspondiente,
para hacerlo verifica que este cumpla la condición de heap con sus hijos*/

void downheap(heap_t* heap, size_t pos_actual){
	_downheap(heap->vector, pos_actual, heap->cantidad, heap->cmp);
}

void _upheap(void* arr[],size_t pos_actual, size_t tope, cmp_func_t cmp){

	if(PADRE(pos_actual) >= tope) return;

	if (cmp(arr[pos_actual], arr[PADRE(pos_actual)]) <= 0) return;

  swap(arr, pos_actual, PADRE(pos_actual));
  _upheap(arr, PADRE(pos_actual), tope, cmp);

}

/*En caso de ser necesario reubica al elemento de la posición indicada en su lugar correspondiente,
para hacerlo verifica que este cumpla la condición de heap con su padre*/

void upheap(heap_t* heap, size_t pos_actual){
	_upheap(heap->vector, pos_actual, heap->cantidad, heap->cmp);
}

/*Ordena inplace el vector recibido tal que este tenga propiedad de heap*/

void heapify(void* elementos[],size_t tope, cmp_func_t cmp){
	for(size_t i = tope/2; i > 0; i--){
		_downheap(elementos, i-1, tope, cmp);
	}
}

/* ******************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {

	heap_t* heap = malloc(sizeof(heap_t));
 	if(!heap) return NULL;

 	heap->vector = malloc(sizeof(void*) * TAMANIO_INICIAL);
 	if(!heap->vector){
 		free(heap);
 		return NULL;
 	}
 	heap->cantidad = 0;
 	heap->tam = TAMANIO_INICIAL;
 	heap->cmp = cmp;
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

	heap_t *heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;

	heap->vector = malloc(sizeof(void*)*n);
	if(!heap->vector){
		free(heap);
		return NULL;
	}

	for(size_t i = 0; i < n; i++){
		heap->vector[i] = arreglo[i];
	}

	heapify(heap->vector, n, cmp);
	heap->cantidad = n;
	heap->tam = n;
	heap->cmp = cmp;

	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *elemento)){

	if(destruir_elemento){
 		for(size_t i = 0; i < heap->cantidad; i++){
 			destruir_elemento(heap->vector[i]);
 		}
 	}
 	free(heap->vector);
 	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap_cantidad(heap) == 0);
}

void *heap_ver_max(const heap_t *heap){

	if(!heap_esta_vacio(heap)){
		return heap->vector[0];
	}
	return NULL;

}

bool heap_encolar(heap_t *heap, void *elem){

	if (!elem) return false;

	size_t pos_elem = heap->cantidad;

  if(pos_elem == heap->tam){
  	if(!heap_redimensionar(heap, heap->tam*FACTOR_REDIMENSION_AUMENTAR)){
      return false;
    }
	}

  heap->vector[pos_elem] = elem;
  heap->cantidad++;

  upheap(heap, pos_elem);
  return true;
}

void *heap_desencolar(heap_t *heap){

  if(heap_esta_vacio(heap)) return NULL;

  void* desencolado = heap->vector[0];

  swap(heap->vector, 0, heap->cantidad-1);
  heap->vector[heap->cantidad-1] = NULL;
  heap->cantidad --;
  downheap(heap, 0);

  if(heap->cantidad >= TAMANIO_INICIAL && heap->cantidad < heap->tam/FACTOR_CHECK_REDUCIR){
    heap_redimensionar(heap, heap->tam/FACTOR_REDIMENSION_REDUCIR);
  }

  return desencolado;
}

/* ******************************************************************
 *                    			HEAP SORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){

	heapify(elementos, cant, cmp);

	for(size_t i = cant-1; i > 0; i--){
		swap(elementos, 0, i);
		_downheap(elementos, 0, i, cmp);
	}
}
