#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "tecla_0.h"
#include "tecla_A.h"

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
    for (int i = 0; i < 4; i++) {
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_OUT);
    }
    for (int i = 0; i < 4; i++) {
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
                return MAPA_TECLAS[i][j];  // Retorna a tecla pressionada
            }
        }
        gpio_put(linhas[i], 0);  // Desativa a linha
    }
    return '\0';  // Retorna '\0' se nenhuma tecla foi pressionada
}






// Função principal
int main() {
    PIO pio = pio0;
    bool ok;
    
    

    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();

    printf("Iniciando a transmissão PIO\n");
    if (ok)
        printf("Clock set to %ld\n", clock_get_hz(clk_sys));

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicializa o teclado matricial
    inicia_teclado();

    
    while (true) {
        char tecla = ler_teclado();  // Lê a tecla pressionada

        switch (tecla)
        {
        case '0':
            aciona_tecla_0(0, pio, sm); // Aciona a tecla 0
            break;

        case 'A':
            aciona_tecla_A(pio, sm); // Aciona a tecla A
        default:
            break;
        }
    }
}
