#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "pio_matrix.pio.h"

//numero de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7

//Teclado matricial
uint linhas[4] = {1,2,3,4}; // Pinos das linhas do teclado matricial
uint colunas[4] = {5,6,7,8}; // Pinos das colunas do teclado matricial


///Matriz para mapear as teclas do teclado matricial 4x4 para os caracteres correspondentes a cada tecla pressionada (0-9, A-D, *, #)  
char MAPA_TECLAS[4][4] = { 
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};


//Função para inicializar os GPIOs do teclado matricial
void inicia_teclado() {
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_init(linhas[i]); //Inicializa o pino GPIO especificado no índice i do array linhas
        gpio_set_dir(linhas[i], GPIO_OUT); //Configuração do modo de operação (direção) do pino GPIO indicado por linhas[i] como uma saída digital
    }
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_init(colunas[i]); //Inicializa o pino GPIO especificado no índice i do array colunas
        gpio_set_dir(colunas[i], GPIO_IN); //Configuração do modo de operação (direção) do pino GPIO indicado por linhas[i] como uma entrada
        gpio_pull_down(colunas[i]); //Ativa resistor de pull-down, quando nenhuma tecla estiver pressionada, o valor lido no pino será "0" (baixo)
    } 
}

//Função para ler o teclado matricial (verifica se alguma tecla foi pressionada)
char ler_teclado(){ //Declara a função chamada ler_teclado que retorna um valor do tipo char, a tecla pressionada
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_put(linhas[i], 1); //Ativa a linha ( valor de 1 (ou HIGH) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
        for (int j = 0; j < 4; j++){ //Repete o bloco de código para os valores de j começando de 0 até 3 (inclusive), incrementando j em 1 a cada iteração
    if (gpio_get(colunas[j])){ //Verifica se a coluna foi ativada
        gpio_put(linhas[i],0); //Desativa a linha (valor de 0 (ou LOW) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
        sleep_ms(50); // Debounce (Espera 50 milissegundos para garantir que o sinal seja estável, evitando leituras falsas)
        return MAPA_TECLAS[i][j]; //Retorna a tecla pressionada
    }
 }
 gpio_put(linhas[i],0); ////Desativa a linha (valor de 0 (ou LOW) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
    }
    return '\0'; //Retorna '\0' se nenhuma tecla foi pressionada
}

//vetor para criar imagem na matriz de led - 1
double seta_cima[25] =   {0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0, 
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};

//vetor para criar imagem na matriz de led - 2
double seta_baixo[25] =   {0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};

//vetor para criar imagem na matriz de led - 3
double letra_x[25] =   {0.3, 0.0, 0.0, 0.0, 0.3,
                        0.0, 0.3, 0.0, 0.3, 0.0, 
                        0.0, 0.0, 0.3, 0.0, 0.0,
                        0.0, 0.3, 0.0, 0.3, 0.0,
                        0.3, 0.0, 0.0, 0.0, 0.3};

//vetor para criar imagem na matriz de led - 4
double quadrado[25] =   {1.0, 0.3, 0.3, 0.3, 1.0,
                        0.3, 0.0, 0.0, 0.0, 0.3, 
                        0.3, 0.0, 0.0, 0.0, 0.3,
                        0.3, 0.0, 0.0, 0.0, 0.3,
                        1.0, 0.3, 0.3, 0.3, 1.0};

//vetor para criar imagem na matriz de led - 5
double carinha_feliz[25] =   {0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0};




int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
