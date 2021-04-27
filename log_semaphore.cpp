#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#define  THREAD_NUM 50
#define NITER 10

sem_t mutex;
pthread_t pthreads[THREAD_NUM];
FILE * fp;
/*-----------------------------------------------------------------*/

void * thread_into_count(void * a){
    fprintf(fp, "Hilo %d empezando\n", (int)pthread_self());
    int i, tmp;

    // for (i = 0; i < REP; i++) {
    //     sem_wait(&mut);

    //     printf("Repeticion numero %d \n", i + 1);

    //     counter = counter - 1;

    //     printf("Id actual: %d - Tomando un recurso \n", id);

    for(i = 0; i < NITER; i++){
        fprintf(fp, "Hilo %d: empezando iteracion %d\n", (int)a, i);
        sem_wait(&mutex);

        fprintf(fp, "Hilo %d: con el recurso \n", (int)a);
        usleep((rand() % 5)*1000000);

        fprintf(fp, "Hilo %d: recurso libre \n", (int)a);
        sem_post(&mutex);

    }
} /*-----------------------------------------------------------------*/

void make_thread_f(){
    for (int i = 0; i < THREAD_NUM; ++i) {
        if(pthread_create(&pthreads[i], NULL, 
            thread_into_count, (void*)i)){
            
            fprintf(fp, "\n Error %d", i);
            exit(1);
        }  /*-----------------------------------------------------------------*/
    }
    pthread_exit(NULL);
}

int main() {
    time_t now;
    char filename[100];
    sprintf( filename, "log_semaphore.txt", ctime(&now));
    /*-----------------------------------------------------------------*/
    fp = fopen (filename,"w");
    srand((unsigned) time(&now));
    sem_init(&mutex, 0, THREAD_NUM);
    make_thread_f();
    fclose(fp);
    return 0;
}