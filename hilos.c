#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *imprimir_decendente(void *arg){
for(int i = 10000; i>=1; i--){
printf("hilo existente %d\n", i);
}
pthread_exit(NULL);
}

int main(){
pthread_t hilo_id;
if(pthread_create(&hilo_id, NULL,imprimir_decendente,NULL) !=0)
{
fprintf(stderr, "Error al crear el hilo\n");
return 1;
}
for(int i = 1; i<=10000; i++){
printf("Proceso principal %d\n", i);}
if(pthread_join(hilo_id, NULL) !=0){
fprintf(stderr, "Error al unir el hilo\n");
return 2;}
return 0;}

