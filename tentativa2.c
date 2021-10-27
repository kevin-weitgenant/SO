#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define LIMITE 30
#define CLIENTES_DIA 500



int cadeiras_ocup = 0;
bool descansando = false;
int tempo_atendimento;
int nao_atendidos = 0;

pthread_mutex_t cadeiras_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t descansando_mtx = PTHREAD_MUTEX_INITIALIZER;

//pthread_cond_t cond_cafe;

int entrada_clientes;



void *entra_cliente(){
    while(1){
        
        pthread_mutex_lock(&cadeiras_mtx);
        pthread_mutex_lock(&descansando_mtx);


        if (descansando == false){
            pthread_mutex_unlock(&descansando_mtx);   // já usou na checagem do if, pode dar unlock   
            
            entrada_clientes = rand()%2+1;
            
            cadeiras_ocup+= entrada_clientes;
            
            if(cadeiras_ocup > LIMITE){
                
                printf("Mais clientes do que o permitido. %d Clientes tiveram que se retirar!\n", cadeiras_ocup - LIMITE);
                nao_atendidos += cadeiras_ocup - LIMITE;
                cadeiras_ocup = LIMITE;
                pthread_mutex_unlock(&cadeiras_mtx);
                sleep(1);
            }
            else if(entrada_clientes >= 0){
            printf("Entraram %d clientes.\nClientes na fila no momento = %d\n",entrada_clientes,cadeiras_ocup);
            pthread_mutex_unlock(&cadeiras_mtx);
            sleep(1);

            }
            
            
            
        }

        else if (descansando){
        
            cadeiras_ocup= 1;
            printf("Cliente interrompe descanso do gerente e vai para fila\t %d\n",cadeiras_ocup);
            descansando = false;
            
            //pthread_cond_signal(&cond_cafe);
            pthread_mutex_unlock(&descansando_mtx);
            pthread_mutex_unlock(&cadeiras_mtx);
            sleep(1);

        }

    

         
           
        

    }
}

void *gerente(){
    while(1){
        
        pthread_mutex_lock(&cadeiras_mtx);
        pthread_mutex_lock(&descansando_mtx);


        if(descansando){
            printf("Gerente continua tomando seu cafézinho na tranquilidade!\n");
            pthread_mutex_unlock(&descansando_mtx);
            pthread_mutex_unlock(&cadeiras_mtx);
        }
        
        /*
        while(descansando){
            printf("Gerente continua tomando seu cafézinho na tranquilidade!\n");
            pthread_cond_wait(&cond_cafe, &descansando_mtx);
        } */
        
        
        else if (cadeiras_ocup > 0 ){
            

            cadeiras_ocup-= 1;
            tempo_atendimento = rand()%8 +5 ;


            if( cadeiras_ocup==0){
                descansando = true;
                
            }

            if(descansando){
                
                pthread_mutex_unlock(&descansando_mtx);
                pthread_mutex_unlock(&cadeiras_mtx);
                //sleep(tempo_atendimento);   
                sleep(1);                
                printf("Gerente atendeu mais um cliente! Tempo empregado:%d\tRestam: %d\n",tempo_atendimento,cadeiras_ocup);
                printf("Ninguém mais na fila, gerente vai tomar um cafézinho\t\n");
            }


            

            else{
                pthread_mutex_unlock(&descansando_mtx);
                pthread_mutex_unlock(&cadeiras_mtx);               
                //sleep(tempo_atendimento); 
                sleep(1); 
                printf("Gerente atendeu mais um cliente! Tempo empregado:%d\tRestam: %d\n",tempo_atendimento,cadeiras_ocup);
            }

                  
        }

        
        
    }
    
}

int main(){
    srand(time(NULL));
    
    
    pthread_t t_entra_cliente, t_gerente;
    pthread_mutex_init(&cadeiras_mtx, NULL);
    pthread_mutex_init(&descansando_mtx, NULL);

    //pthread_cond_init(&cond_cafe, NULL);

    pthread_create(&t_entra_cliente, NULL, &entra_cliente, NULL);
    pthread_create(&t_gerente, NULL, &gerente, NULL);



    pthread_join(t_entra_cliente, NULL);
    pthread_join(t_gerente, NULL);

    pthread_mutex_destroy(&cadeiras_mtx);
    pthread_mutex_destroy(&descansando_mtx);
    //pthread_cond_destroy(&cond_cafe);
    return 0;
    
}
