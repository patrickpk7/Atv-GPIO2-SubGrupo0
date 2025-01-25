#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

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

// Vetores para desenhos na matriz de LED
double seta_baixo[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

double seta_cima[25] = {
    0.0, 0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0, 0.0,
    0.0, 0.5, 0.5, 0.5, 0.0,
    0.0, 0.0, 0.5, 0.0, 0.0};

double letra_x[25] = {
    0.7, 0.0, 0.0, 0.0, 0.7,
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.0, 0.0, 0.3, 0.0, 0.0,
    0.0, 0.7, 0.0, 0.7, 0.0,
    0.7, 0.0, 0.0, 0.0, 0.7};

double quadrado[25] = {
    0.5, 0.5, 0.5, 0.5, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.5, 0.5, 0.5, 0.5, 0.5};

double losango[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.3, 0.0, 0.0, 0.0, 0.3,
    0.0, 0.5, 0.0, 0.5, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função principal
int main() {
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    

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

        if (tecla == '0') {  // Verifica se a tecla '0' foi pressionada
            desenho_pio(seta_baixo, valor_led, pio, sm, 1.0, 0.0, 0.0);  // Vermelho
            sleep_ms(500);
            desenho_pio(seta_cima, valor_led, pio, sm, 0.0, 0.0, 1.0);  // Azul
            sleep_ms(500);
            desenho_pio(letra_x, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
            sleep_ms(500);
            desenho_pio(quadrado, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Branco
            sleep_ms(500);
            desenho_pio(losango, valor_led, pio, sm, 1.0,0.0,1.0);  // Multicolorido
            sleep_ms(500); 
            }
        }
    }
