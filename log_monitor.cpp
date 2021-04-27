#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 5

pthread_mutex_t mutex;
unsigned available_resources;



int increase_count(int count){
	pthread_mutex_lock(&mutex);
	printf("Se bloquea Mutex \n");
	printf("Cantidad de %d recursos \n", count);
	available_resources = available_resources + count;
	pthread_mutex_unlock(&mutex);
	printf("Desbloqueo de Mutex \n");
	return 0;
}

int decrease_count(int count){
	if (available_resources < count) {
		return -1;

	}else{
		pthread_mutex_lock(&mutex);
		printf("Bloquear Mutex \n");
		printf("Se estan consumient %d \n", count, "recursos");
		available_resources = available_resources - count;
		pthread_mutex_unlock(&mutex);
		printf("Desbloqueo de Mutex \n");
		return 0;
	}
}


void* manager(void) {
	int i;
	pid_t id;
	id = syscall(SYS_gettid);
	printf("Thread proces id %d \n", id);
	int res_consume = (rand() % 11) + 5;
	printf("Id : %d - y se usan %d  \n", "recursos",id, res_consume);

	while (decrease_count(res_consume) != 0) {
		NULL;
	}

	decrease_count(res_consume);
	int time = (rand()%2)+1;
	sleep(time);
	printf("Id : %d - y se usan %d  \n", id, res_consume, "recursos");
	increase_count(res_consume);
	printf("Id : %d - Retornan %d recursos \n", id, res_consume);

}

int main() {
	FILE *fptr;
	fptr = fopen("log_monitor.txt", "w");

	if (fptr == NULL){
		printf("Error!");
		exit(1);
	}

	fprintf(fptr, "Inicio \n");
	int j;
	pthread_t *thread;

	available_resources = 20;

	fprintf(fptr, "Realizando %d threads\n", THREADS);
	for (j = 0; j < THREADS; j++) {
		pthread_create(&thread[j], NULL, manager, NULL);
	}

	fprintf(fptr, "Esperando ... \n");
	for (j = 0; j < THREADS; j++) {
		pthread_join(thread[j], NULL);
	}

	fprintf(fptr, "Final \n");
	fclose(fptr);
	return 0;
}