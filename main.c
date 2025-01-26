// Inclui as bibliotecas necessárias para o funcionamento do código
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// Inclui os arquivos .h das funções de cada tecla
#include "Tecla_A.h"
#include "Tecla_B.h"
#include "Tecla_C.h"
#include "Tecla_D.h"
#include "Tecla_0.h"
#include "Tecla_1.h"
#include "Tecla_2.h"
#include "Tecla_3.h"
#include "Tecla_4.h"
#include "Tecla_5_#.h"
#include "Tecla_6.h"
#include "Tecla_7.h"   
#include "Tecla_8.h"

// Arquivo .pio
#include "pio_matrix.pio.h"

// Número de LEDs
#define NUM_PIXELS 25

// Pino de saída
#define OUT_PIN 7

// Teclado matricial
uint linhas[4] = {1, 2, 3, 4};    // Pinos das linhas do teclado matricial
uint colunas[4] = {5, 6, 8, 9};   // Pinos das colunas do teclado matricial

// Mapa de teclas
char MAPA_TECLAS[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Inicializa o teclado matricial
void inicia_teclado() {
    for (int i = 0; i < 4; i++) { // Configura as linhas como saída
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_OUT);
    }
    for (int i = 0; i < 4; i++) { // Configura as colunas como entrada
        gpio_init(colunas[i]);
        gpio_set_dir(colunas[i], GPIO_IN);
        gpio_pull_down(colunas[i]);
    }
}

// Lê o teclado matricial
char ler_teclado() {
    for (int i = 0; i < 4; i++) {
        gpio_put(linhas[i], 1);  // Ativa a linha
        sleep_ms(1);  // Delay para estabilizar o sinal
        for (int j = 0; j < 4; j++) {
            if (gpio_get(colunas[j])) {  // Verifica se a coluna foi ativada
                gpio_put(linhas[i], 0);  // Desativa a linha
                sleep_ms(50);  // Debounce
                char tecla = MAPA_TECLAS[i][j]; 
                //printf("Tecla pressionada: %c\n", tecla); // Exibe a tecla pressionada
                return MAPA_TECLAS[i][j];  // Retorna a tecla pressionada
            }
        }
        gpio_put(linhas[i], 0);  // Desativa a linha
    }
    return '\0';  // Retorna '\0' se nenhuma tecla foi pressionada
}






// Função principal
int main() {
    PIO pio = pio0; // Define o pio como pio0 
    bool ok; // Variável para verificar se o clock foi setado corretamente
    
    
    // Configura o clock do sistema para 128MHz (máximo)
    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();

    printf("Terminal Iniciado.\n Aguardando teclas....\n");
    if (ok) // Verifica se o clock foi setado corretamente
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));

    // Configura a matriz de LEDs
    uint offset = pio_add_program(pio, &pio_matrix_program); // Adiciona o programa .pio
    uint sm = pio_claim_unused_sm(pio, true); // Reivindica o bloco de estado de máquina do pio
    pio_matrix_program_init(pio, sm, offset, OUT_PIN); // Inicializa o programa .pio

    // Inicializa o teclado matricial
    inicia_teclado();

    // Loop principal
    while (true) {
        char tecla = ler_teclado();  // Lê a tecla pressionada

        switch (tecla)
        {
        case '0':
            printf("Tecla 0 pressionada: Exibindo animação\n");
            aciona_tecla_0(0, pio, sm); // Exibe formas geometricas
            break;
        case '1':
            printf("Tecla 1 pressionada: Exibindo animação\n");
            animacao_X(0, pio, sm); // Exibe a animação do "X"
            break;
        case '2':
            printf("Tecla 2 pressionada: Exibindo animação\n");
            animacao_CORACAO(0, pio, sm); // Exibe a animação do coração
            break;
        case '3':
            printf("Tecla 3 pressionada: Exibindo animação\n");
            animacao_ALINE(0, pio, sm); // Exibe a animação da palavra "ALINE"
            break;
        case '4':
            printf("Tecla 4 pressionada: Exibindo animação\n");
            animacao_tecla4(0, pio, sm); // Exibe a animação de sinais matematicos e formas geometricas
            break;
        case '5':
            printf("Tecla 5 pressionada: Exibindo animação\n");
            funcao_tecla_5(); // Aciona a tecla 5
            break;
        case '6':
            printf("Tecla 6 pressionada: Exibindo animação\n");
            aciona_tecla_6(0, pio, sm); // Exibe a animação de numeros 1 a 5
            break;
        case '7':
            printf("Tecla 7 pressionada: Exibindo animação\n");
            tecla_7(0, pio, sm); // Exibe a animação de emojis feliz e triste
            break;
        case '8':
            printf("Tecla 8 pressionada: Exibindo animação\n");
            aciona_tecla_8(0, pio, sm); // Exibe animação de casa, escada, cruz, letra O e quadrado preenchido
            break;
        case 'A': // Desliga os LEDs
            printf("Tecla A pressionada: Desligando LEDS\n");
            aciona_tecla_A(pio, sm); // Aciona a tecla A
            break;
        case 'B': // Liga os LEDs em azul com 100% de intensidade
            printf("Tecla B pressionada: Ligando LEDS AZUIS\n");
            aciona_tecla_B(pio, sm); // Aciona a tecla B
            break;
        case 'C': // Liga os LEDs em vermelho com 80% de intensidade
            printf("Tecla C pressionada: Ligando LEDS VERMELHOS\n");
            ligar_vermelho(pio, sm); // Exibe a animação do LED
            break;
        case 'D': // Liga os LEDs em verde com 50% de intensidade
            printf("Tecla D pressionada: Ligando LEDS VERDES\n");
            ligar_verde(pio, sm); // Aciona a tecla D
            break;
        case '#': // Liga os LEDs em branco com 20% de intensidade
            printf("Tecla # pressionada: Ligando LEDS BRANCOS\n");
            funcao_tecla_hashtag(pio, sm); // Aciona a tecla #
            break;
        case '*':
            printf("Modo BOOTSEL ATIVADO\n");
            reset_usb_boot(0, 0); // Entra no modo BOOTSEL
            break;
        default:
            break;
        }
    }
}
