#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define LIMITE 3
#define CLIENTES_DIA 10

int catraca = 0;
int cadeiras_ocup = 0;
bool descansando = false;
int tempo_atendimento;
int nao_atendidos = 0;

pthread_mutex_t cadeiras_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t descansando_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_cafe;
pthread_t t_entra_cliente, t_gerente;

int entrada_clientes;


void *entra_cliente(){   
    while(catraca <= CLIENTES_DIA){
        
        pthread_mutex_lock(&cadeiras_mtx);
        pthread_mutex_lock(&descansando_mtx);


        if (descansando == false){
            pthread_mutex_unlock(&descansando_mtx);   // já usou na checagem do if, pode dar unlock  
            
            entrada_clientes = rand()%2+1;            
            cadeiras_ocup+= entrada_clientes;
            catraca+= entrada_clientes;            
            if(cadeiras_ocup > LIMITE){                
                printf("Mais clientes do que o permitido. Dos %d Clientes que entraram, %d tiveram que se retirar!\n", entrada_clientes,cadeiras_ocup - LIMITE);
                nao_atendidos += cadeiras_ocup - LIMITE;
                cadeiras_ocup = LIMITE;                
                pthread_mutex_unlock(&cadeiras_mtx);
                sleep(1);
            }else if(entrada_clientes >= 0){
                printf("Entraram %d clientes.\nClientes no estabelecimento no momento = %d\n",entrada_clientes,cadeiras_ocup);
                pthread_mutex_unlock(&cadeiras_mtx);
                sleep(rand()%30+1);
            }   
        }else if(descansando){        
            cadeiras_ocup= 1;
            printf("Cliente interrompe descanso do gerente e vai para fila\t \n");
            descansando = false;
            pthread_mutex_unlock(&descansando_mtx);
            pthread_mutex_unlock(&cadeiras_mtx);
            pthread_cond_signal(&cond_cafe);
            sleep(1);
        }
    }
    printf("\n\nNúmero máximo de clientes por hoje!\n\n\n");
    catraca--;
    pthread_exit(&t_entra_cliente);
}


void *gerente(){  
    while(1){        
        pthread_mutex_lock(&cadeiras_mtx);
        pthread_mutex_lock(&descansando_mtx);
                
        while(descansando){
            printf("Gerente continua tomando seu cafézinho na tranquilidade!\n");
            pthread_mutex_unlock(&cadeiras_mtx);
            pthread_mutex_unlock(&descansando_mtx); 
            pthread_cond_wait(&cond_cafe, &descansando_mtx);           
        }  
        if(cadeiras_ocup > 0){  
            tempo_atendimento = rand()%8 + 5;
            printf("Gerente começa o atendimento de um cliente\n");
            pthread_mutex_unlock(&descansando_mtx);
            pthread_mutex_unlock(&cadeiras_mtx);
            sleep(tempo_atendimento);               
          
            pthread_mutex_lock(&cadeiras_mtx);              
            pthread_mutex_lock(&descansando_mtx);
            cadeiras_ocup-= 1;  
            pthread_mutex_unlock(&descansando_mtx);

            printf("Gerente atendeu mais um cliente! Tempo empregado:%d\tRestam: %d\n",tempo_atendimento,cadeiras_ocup);
            pthread_mutex_unlock(&cadeiras_mtx);

            pthread_mutex_lock(&descansando_mtx);
            pthread_mutex_lock(&cadeiras_mtx);
            

            if(cadeiras_ocup == 0 && catraca < CLIENTES_DIA){
                descansando = true;
                printf("Ninguém mais na fila, gerente vai tomar um cafézinho\t\n");
                pthread_mutex_unlock(&descansando_mtx);
                pthread_mutex_unlock(&cadeiras_mtx);
            }else if(cadeiras_ocup == 0 && catraca >= CLIENTES_DIA){
                printf("\n\nÚltimo cliente do dia atendido! Fim de mais um insuportável dia de trabalho\n\n");
                printf("Limite na Catraca = %d\n", CLIENTES_DIA);
                printf("Clientes atendidos = %d\n", CLIENTES_DIA - nao_atendidos );
                printf("Clientes não atendidos = %d\n",nao_atendidos);
                pthread_exit(&t_gerente);
            }else{
                pthread_mutex_unlock(&descansando_mtx);
                pthread_mutex_unlock(&cadeiras_mtx);
            }          
        }
    } 
}


int main(){
    srand(time(NULL));        
    
    pthread_mutex_init(&cadeiras_mtx, NULL);
    pthread_mutex_init(&descansando_mtx, NULL);

    pthread_cond_init(&cond_cafe, NULL);
   
    pthread_create(&t_entra_cliente, NULL, &entra_cliente, NULL);   
    pthread_create(&t_gerente, NULL, &gerente, NULL);

    pthread_join(t_entra_cliente, NULL);
    pthread_join(t_gerente, NULL);

    pthread_mutex_destroy(&cadeiras_mtx);
    pthread_mutex_destroy(&descansando_mtx);

    pthread_cond_destroy(&cond_cafe);
    return 0;
}