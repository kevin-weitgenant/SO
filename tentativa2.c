#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

const int LIMITE = 20;

int cadeiras_ocup = 0;
int descansando = 0;
int tempo_atendimento;
int temp;

pthread_mutex_t cadeiras_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t descansando_mtx = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t cond;

int entrada_clientes;

void *entra_cliente(){
    while(1){      
        if (descansando == 0){
            //printf("\t\t\tantes de dar lock 1loop1!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            entrada_clientes = rand()%2;
            cadeiras_ocup+= entrada_clientes;                   

            if(cadeiras_ocup <= LIMITE && entrada_clientes != 0){                
                printf("Entraram %d clientes.\nClientes na fila no momento = %d\n",entrada_clientes,cadeiras_ocup);  
            }else if (entrada_clientes != 0){
                printf("Entraram %d clientes.\nClientes na fila no momento = %d\n",entrada_clientes,cadeiras_ocup);
                temp = cadeiras_ocup - LIMITE;
                cadeiras_ocup -= temp;                
                printf("Então %d clientes foram removidos imediatamente da fila !\n", temp);                
            }            

            pthread_mutex_unlock(&cadeiras_mtx);
            sleep(1);
            //printf("\t\t\tUNLOCK 1LOOP1\n");
        }

        else if (descansando == 1 && cadeiras_ocup == 0 ){
        
            //printf("\t\t\tantes de dar lock 1loop2!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            pthread_mutex_lock(&descansando_mtx);
            //printf("Cliente interrompe descanso do gerente e vai para fila.\t sao %d clientes\n",cadeiras_ocup);
            descansando = 0;
            pthread_cond_signal(&cond);
            entrada_clientes = rand()%4;
            cadeiras_ocup+= entrada_clientes;
            printf("Cliente interrompe descanso do gerente e vai para fila.\t sao %d clientes\n",cadeiras_ocup);


            pthread_mutex_unlock(&cadeiras_mtx); 
            pthread_mutex_unlock(&descansando_mtx);
            sleep(1);
            //printf("\t\t\tUNLOCK 1LOOP2\n");

        }

    

        //printf("\t\t\tentrou loop1!\n");
    printf("\n");  
    }
}

void *gerente(){
    while(1){
        
        while(cadeiras_ocup <= 0 && descansando == 1){
            printf("Gerente continua tomando seu cafézinho na tranquilidade!\n");
            pthread_cond_wait(&cond, &descansando_mtx);

        }
        
        
        if (cadeiras_ocup > 0 ){
            //printf("\t\t\tantes de dar lock 2loop1!\n");
            pthread_mutex_lock(&cadeiras_mtx);
            pthread_mutex_lock(&descansando_mtx);

            cadeiras_ocup-= 1;
            tempo_atendimento = rand()%8 +5 ;
            //sleep(tempo_atendimento);
            sleep(1);
            printf("Gerente atendeu mais um cliente! Tempo empregado:%d\tRestam: %d\n\n",tempo_atendimento,cadeiras_ocup);
            


            if( cadeiras_ocup<=0){
              descansando = 1;
              printf("Ninguém mais na fila, gerente vai tomar um cafézinho\t%d\n",cadeiras_ocup);
            }
            pthread_mutex_unlock(&cadeiras_mtx);
            pthread_mutex_unlock(&descansando_mtx);
            //printf("\t\t\tUNLOCK 2LOOP1\n");

            sleep(1);
        }

        //printf("\t\t\tpassou loop2\n");
    printf("\n"); 
    }
    
}

int main(){
    srand(time(NULL));
    
    
    pthread_t t_entra_cliente, t_gerente;
    pthread_mutex_init(&cadeiras_mtx, NULL);
    pthread_mutex_init(&descansando_mtx, NULL);

    pthread_cond_init(&cond, NULL);

    //pthread_create(&t_gerente, NULL, &gerente, NULL);
    pthread_create(&t_entra_cliente, NULL, &entra_cliente, NULL);
    pthread_create(&t_gerente, NULL, &gerente, NULL);



    pthread_join(t_entra_cliente, NULL);
    pthread_join(t_gerente, NULL);

    pthread_mutex_destroy(&cadeiras_mtx);
    pthread_mutex_destroy(&descansando_mtx);
    pthread_cond_destroy(&cond);
    return 0;
    
}