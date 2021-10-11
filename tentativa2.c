#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>




int cadeiras_ocup = 20;
int descansando =0;
const int LIMITE = 25;

pthread_mutex_t cadeiras_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t descansando_mtx = PTHREAD_MUTEX_INITIALIZER;


void *entra_cliente(){
    while(1){
        
        if (descansando == 0){
            printf("\t\t\tantes de dar lock 1loop1!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            cadeiras_ocup+= 5;
            
            if(cadeiras_ocup > LIMITE){
                printf("Mais clientes do que o permitido. Cliente teve que se retirar!\n");
                cadeiras_ocup--;
            }
            printf("Clientes na fila no momento = %d\n",cadeiras_ocup);
            pthread_mutex_unlock(&cadeiras_mtx);
            sleep(1);
            printf("\t\t\tUNLOCK 1LOOP1\n");
        }

        else if (descansando == 1){
        
            printf("\t\t\tantes de dar lock 1loop2!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            pthread_mutex_lock(&descansando_mtx);
            printf("Cliente interrompe descanso do gerente e vai para fila\t %d\n",cadeiras_ocup);
            descansando = 0;
            cadeiras_ocup+= 1;


            pthread_mutex_unlock(&cadeiras_mtx); 
            pthread_mutex_unlock(&descansando_mtx);
            sleep(1);
            printf("\t\t\tUNLOCK 1LOOP2\n");

        }

    

        printf("\t\t\tentrou loop1!\n");

    }
}

void *gerente(){
    while(1){
        if (cadeiras_ocup > 0 ){
            printf("\t\t\tantes de dar lock 2loop1!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            pthread_mutex_lock(&descansando_mtx);

            cadeiras_ocup-= 1;
            printf("Gerente atendeu mais um cliente! Restam: %d\n",cadeiras_ocup);
            if( cadeiras_ocup<=0){
                descansando = 1;
                printf("Ninguém mais na fila, gerente vai tomar um cafézinho\t%d\n",cadeiras_ocup);
            }
            pthread_mutex_unlock(&cadeiras_mtx);
            pthread_mutex_unlock(&descansando_mtx);
            printf("\t\t\tUNLOCK 2LOOP1\n");

            sleep(1);
        }

        else if(cadeiras_ocup <= 0 && descansando == 1){
            printf("Gerente continua tomando seu cafézinho na tranquilidade!\n");
            sleep(1);
        }
        


        printf("\t\t\tpassou loop2\n");
    }
    
}

int main(){

    pthread_t t_entra_cliente, t_gerente;
    pthread_mutex_init(&cadeiras_mtx, NULL);
    pthread_mutex_init(&descansando_mtx, NULL);

    pthread_create(&t_entra_cliente, NULL, &entra_cliente, NULL);
    pthread_create(&t_gerente, NULL, &gerente, NULL);



    pthread_join(t_entra_cliente, NULL);
    pthread_join(t_gerente, NULL);

    pthread_mutex_destroy(&cadeiras_mtx);
    pthread_mutex_destroy(&descansando_mtx);
    return 0;

}