/**
 * Problema do barbeiro dorminhoco
 * 
 * Professor: Augusto Silva
 * Displina: Enfase II - Threads
 * PUC-GOIAS
 * 
 * @author Silas dos S. Silva
 * @since 08 dez 2014 14:56
 * 
 * 
 * DETALHES DA COMPILACAO
 * 
 * Sistema operacional: Debian GNU/Linux jessie/sid
 * IDE: Netbeans 8.0
 * Compilador: gcc (Debian 4.8.2-16) 4.8.2
 * Parametros de compilacao (adicionados a ide): -lpthread -lm
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

// Maximo de cadeiras - qtd maxima de clientes que aguardam o barbeiro
const int CADEIRAS = 5;

// Qtd de clientes que estao aguardando nas cadeiras
int qtdClientesAguardando = 0;

// Informacoes do cliente que sera atendido (neste caso so inclui o ID)
struct strcCliente {
    int id;
};

// Semaforos
sem_t clientes;
sem_t barbeiro;
sem_t mutex;

// Definicao de funcoes
void* pthBarbeiro();
void* pthCliente(int *param);
double distNormalAleatoria();

// cabecalho do projeto
void cabecalho() {
    // Limpar a tela do terminal no GNU linux
    printf("\e[1;1H\e[2J");
    printf("+---------------------------------------------------------------+\n");
    printf("|        PONTIFICIA UNIVERSIDADE CATOLICA DE GOIAS              |\n");
    printf("| Aluno: Silas dos S. Silva          Matricula: 2011.1.033.0188 |\n");
    printf("|              O PROBLEMA DO BARBEIRO DORMINHOCO                |\n");
    printf("+---------------------------------------------------------------+\n");
    printf("| Quantidade de Cadeiras disponíveis: %d                         |\n", CADEIRAS);
    printf("| Taxa de chegada de clientes: Distribuição normal de 1.0 a 3.0 |\n");
    printf("| Tempo de atendimento do barbeiro: 1 cliente a cada 3 segundos |\n\n");
}

int main() {
    cabecalho();
    
    // o 1 Indica que compartilha com todas as trheads, sera alocado na regiao de memoria pertencente ao processo
    sem_init(&clientes, 1, 0);
    sem_init(&barbeiro, 1, 0);
    sem_init(&mutex, 1, 1);
    
    srand(time(NULL));

    // Identificadores das threads
    pthread_t tBarbeiro, tCliente;

    // Acionar a thread do barbeiro
    pthread_create(&tBarbeiro, NULL, (void *) pthBarbeiro, NULL);
    
    // Criar um cliente a cada x segundos (distribuicao Normal)
    int i = 0;
    while (1) {
        struct strcCliente cliente;
        cliente.id = ++i;
        
        // thread, attr pthread, start routine, *args
        pthread_create(&tCliente, NULL, (void *) pthCliente, &cliente.id);
        
        // Aplicar distribuicao normal na criacao de clientes - gerar um numero randomico entre 1.0 e 3.0, ex.: 1.7 ; 2.8
        sleep(distNormalAleatoria());
    }

    return 0;
}

void* pthCliente(int *param) {
    // sem_wait: Utilizada para fazer um P/down sobre um semaforo, ou seja, decrementa seu valor e bloqueia se o valor estava em zero
    sem_wait(&mutex);

    if (qtdClientesAguardando < CADEIRAS) {
        // Cliente vai aguardar atendimento
        printf("\nCliente novo. Agora temos %d clientes em cadeiras.\n", (*clientes.__size)+1);
        
        qtdClientesAguardando++;
        
        // sem_post: Utilizada para fazer um V/up sobre o semaforo, ou seja, incrementa o seu valor. 
        // Se o semaforo valia 0 e algum processo/thread estava bloqueado neste semaforo entao sera liberado e podera executar quando selecionado pelo escalonador
        // Liberando
        sem_post(&clientes); 
        sem_post(&mutex);
        
        // Bloqueando
        sem_wait(&barbeiro);
        
        printf("Cliente numero %d está sendo atendido pelo barbeiro.\n", *param);
    }
    else {
        // Liberando
        sem_post(&mutex);
        
        printf("\nCliente numero %d foi embora, fila muito grande.\n", *param);
    }

    pthread_exit(NULL);
}

// Esta sera usada como thread do barbeiro, e nunca atingira pthread_exit, sempre que houver cliente na fila ele atende, quando nao, dorme.
void* pthBarbeiro() {
    while(1) {
        // Bloqueando
        sem_wait(&clientes);
        sem_wait(&mutex);
        
        qtdClientesAguardando--;
        
        // Ativar
        sem_post(&barbeiro);
        sem_post(&mutex);
        
        // Barbeiro atendendo
        printf("Barbeiro atendendo o cliente. Sobraram %d clientes aguardando em cadeiras.\n", qtdClientesAguardando);
        
        // Aguarda 3 segundos ate chamar o proximo cliente
        sleep(3);
    }

    pthread_exit(NULL);
}

/*
 * Minha funcao geradora da distribuicao normal, range 1.0 - 3.0
 */
double distNormalAleatoria() {
    double res;
    do {
      res = 1.0 + 1.5 * sqrt(-2 * log((rand()+1.0) / (RAND_MAX+1.0))) * cos(2 * M_PI * (rand()+1.0) / (RAND_MAX+1.0));
    }
    while (res <= 1.0 || res >= 3.0);
    
    return res;
}
