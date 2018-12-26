#include "heap.h"
#include "testing.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM_VECTOR 7

/*FUNCION COMPARAR*/

int cmp(const void *a, const void *b){

	if(a == NULL) return -1;
	if(b == NULL) return 1;

	if (*(int*)a > *(int*)b){
		return 1;
	}else if(*(int*)a == *(int*)b){
		return 0;
	}else{
		return -1;
	}
}

/*FUNCIONES DE PRUEBAS*/


void prueba_heap_vacio(){

	heap_t* heap = heap_crear(NULL);
	print_test("Heap creado" , heap != NULL);
	print_test("La cantidad de elementos es cero" , heap_cantidad(heap) == 0);
	print_test("Heap esta vacio" , heap_esta_vacio(heap) == true);
	print_test("Heap ver maximo. Debe dar NULL" , heap_ver_max(heap) == NULL);
	print_test("Heap desencolar. Debe dar NULL" , heap_desencolar(heap) == NULL);
	heap_destruir(heap , NULL);

}

void prueba_heap_encolar(){

  heap_t *heap = heap_crear(cmp);
  int num1 = 3;
  int num2 = 7;
  int num3 = 0;
	int num4 = 3;

  print_test("Insertar num1 = 3", heap_encolar(heap, &num1));
  print_test("Comprobando si esta vacio. Debe dar false", !heap_esta_vacio(heap));
  print_test("La cantidad de elementos debe ser 1", heap_cantidad(heap) == 1);
  print_test("Viendo maximo. Debe ser num1", heap_ver_max(heap) == &num1);
  print_test("Insertar num2 = 7", heap_encolar(heap, &num2));
  print_test("Insertar num3 = 0", heap_encolar(heap, &num3));
  print_test("La cantidad de elementos debe ser 3", heap_cantidad(heap) == 3);
  print_test("Viendo maximo. Debe ser num2", heap_ver_max(heap) == &num2);
	print_test("insertar NULL es invalido", heap_encolar(heap, NULL) == false);
	print_test("Insertar elemento igual (num4) a un elemento del heap", heap_encolar(heap, &num4));
	//num4 quedará como último elemento del heap
	print_test("Desencolo primer elemento", heap_desencolar(heap) == &num2);
	//al desencolar y hace swap con el último elemento num4 queda como tope
	print_test("Viendo maximo. Debe ser num4", heap_ver_max(heap) == &num4);
	print_test("La cantidad de elementos debe ser 3", heap_cantidad(heap) == 3);

	for(int i = 0; i < 3; i++){
		heap_desencolar(heap);
	}

	print_test("heap esta vacio", heap_esta_vacio(heap) == true);
  heap_destruir(heap, NULL);
}


void prueba_heap_desencolar(){

	heap_t *heap = heap_crear(cmp);
  int num1 = 3;
  int num2 = 7;
  int num3 = 0;

  print_test("Insertar num1 = 3", heap_encolar(heap, &num1));
  print_test("Comprobando si esta vacio. Debe ser false", heap_esta_vacio(heap) == false);
  print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 1);
  print_test("Ver maximo. Debe ser num1", heap_ver_max(heap) == &num1);
  print_test("Insertar num2 = 7", heap_encolar(heap, &num2));
  print_test("Insertar num3 = 0", heap_encolar(heap, &num3));
  print_test("La cantidad de elementos es 3", heap_cantidad(heap) == 3);
  print_test("Ver maximo. Debe ser num2", heap_ver_max(heap) == &num2);

  print_test("Desencolar. Debe ser num2", heap_desencolar(heap) == &num2);
  print_test("La cantidad de elementos es 2", heap_cantidad(heap) == 2);
  print_test("Desencolar. Debe ser num1", heap_desencolar(heap) == &num1);
  print_test("La cantidad de elementos es 1", heap_cantidad(heap) == 1);
  print_test("Desencolar. Debe ser num3", heap_desencolar(heap) == &num3);
  print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);
  print_test("Esta vacio. Debe ser true", heap_esta_vacio(heap) == true);
  print_test("Ver maximo. Debe ser NULL", heap_ver_max(heap) == NULL);
  print_test("Desencolar. Debe ser NULL", heap_desencolar(heap) == NULL);
  heap_destruir(heap, NULL);
}


void prueba_heap_destruir(){

  heap_t *heap = heap_crear(cmp);
  int *num1 = malloc(sizeof(int));
  int *num2 = malloc(sizeof(int));
  int *num3 = malloc(sizeof(int));
  *num1 = 3;
  *num2 = 0;
  *num3 = 7;

  print_test("Insertar num1 = 3", heap_encolar(heap, num1));
  print_test("Insertar num2 = 0", heap_encolar(heap, num2));
  print_test("Insertar num3 = 7", heap_encolar(heap, num3));
  print_test("Ver maximo. Debe ser num3", heap_ver_max(heap) == num3);
  print_test("La cantidad de elementos es 3", heap_cantidad(heap) == 3);
  print_test("Esta vacio. Debe ser false", heap_esta_vacio(heap) == false);
  print_test("Desencolar. Debe ser num3", heap_desencolar(heap) == num3);
  free(num3);
  heap_destruir(heap, free);
  print_test("Prueba heap destruir", true);
}

void swap_vector_prueba(int **vector, size_t pos1, size_t pos2){

	int* aux = vector[pos1];
	vector[pos1] = vector[pos2];
	vector[pos2] = aux;
}

void shuffle(int** arr, size_t n){

  srand((unsigned)time(NULL));
  for(size_t i = n; i > 0; i--){
    size_t pos = (size_t)rand()%i;
    swap_vector_prueba(arr,i-1,pos);
  }
}

void prueba_heap_volumen(size_t cant_elementos){

  heap_t *heap = heap_crear(cmp);
  int* vector[cant_elementos];
	bool encolar_ok = true;
  for (int i = 0; i < cant_elementos; i++) {
    int *puntero = malloc(sizeof(int));
    *puntero = i;
    vector[i] = puntero;
  }
  shuffle(vector, cant_elementos);
  for (int i = 0; i < cant_elementos; i++) {
  	encolar_ok &= heap_encolar(heap, vector[i]);
  }
  printf("Encolar lote de %lu elementos" , cant_elementos);
  print_test("" , encolar_ok == true);
  while(heap_cantidad(heap) > cant_elementos/4){
    free(heap_desencolar(heap));
  }
  heap_destruir(heap, free);
  print_test("Prueba heap volumen", true);
}

void imprimir_vector(int** vector,size_t n){
	for(int i = 0; i < n ; i++){
		printf("%d " , *vector[i]);
	}
}

void prueba_heap_sort(){

  int uno = 1;
  int tres = 3;
  int cinco = 5;
  int seis = 6;
  int siete = 7;
  int ocho = 8;
  int nueve = 9;
  int* vector_prueba[TAM_VECTOR] = {&seis, &tres, &cinco, &ocho, &uno, &nueve, &siete};
	int* vector_ordenado[TAM_VECTOR] ={&uno, &tres, &cinco, &seis, &siete, &ocho, &nueve};
  printf("antes de heapsort\n");
	imprimir_vector(vector_prueba, TAM_VECTOR);

  heap_sort((void**)vector_prueba, TAM_VECTOR , cmp);
  printf("\ndespues de heapsort\n");
	imprimir_vector(vector_prueba, TAM_VECTOR);
	printf("\n");
	bool ordenado = true;
	for(int i = 0; i < TAM_VECTOR ; i++){
  	if(vector_prueba[i] != vector_ordenado[i]){
    	ordenado = false;
    }
  }
	print_test("Vector ordenado correctamente", ordenado);
}

void prueba_crear_arr(){
	int uno = 1, dos = 2, tres = 3, cuatro = 4, cinco = 5;
	int* vector[5]={&uno,&dos,&tres,&cuatro,&cinco};
	heap_t* heap = heap_crear_arr((void**)vector, 5, cmp);
	print_test("la cantidad es 5", heap_cantidad(heap) == 5);
	print_test("Desencolar devuelve 5", heap_desencolar(heap) == vector[4]);
	print_test("la cantidad es 4", heap_cantidad(heap) == 4);
	print_test("Desencolar devuelve 4", heap_desencolar(heap) == vector[3]);
	print_test("la cantidad es 3", heap_cantidad(heap) == 3);
	print_test("Desencolar devuelve 3", heap_desencolar(heap) == vector[2]);
	print_test("la cantidad es 2", heap_cantidad(heap) == 2);
	print_test("Desencolar devuelve 2", heap_desencolar(heap) == vector[1]);
	print_test("ultimo elemento es 1", heap_cantidad(heap) == 1  && heap_desencolar(heap) == vector[0]);
	print_test("La cantidad de elementos es cero" , heap_cantidad(heap) == 0);
	print_test("Heap esta vacio" , heap_esta_vacio(heap) == true);
	print_test("Heap ver maximo. Debe dar NULL" , heap_ver_max(heap) == NULL);
	print_test("Heap desencolar. Debe dar NULL" , heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}
void pruebas_heap_alumno(void){

	printf("PRUEBA HEAP VACIO\n\n");
	prueba_heap_vacio();
	printf("\nPRUEBA HEAP ENCOLAR\n\n");
prueba_heap_encolar();
	printf("\nPRUEBA HEAP DESENCOLAR\n\n");
	prueba_heap_desencolar();
	printf("\nPRUEBA HEAP DESTRUIR\n\n");
  prueba_heap_destruir();
  printf("\nPRUEBA HEAP DE VOLUMEN\n\n");
  prueba_heap_volumen(800);
  printf("\nPRUEBA HEAPSORT\n\n");
	prueba_heap_sort();
	printf("\nPRUEBA HEAP CREAR CON ARREGLO\n\n");
	prueba_crear_arr();

}
