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



// Vetores para as letras de "ALINE"
double letra_A[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double letra_L[25] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 1.0};

double letra_I[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0};

double letra_N[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 1.0, 1.0,
    1.0, 0.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 0.0, 0.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 1.0};

double letra_E[25] = {
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0};

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb3(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para corrigir a ordem dos LEDs (inverter as linhas)
int corrigir_indice(int index) {
    int row = index / 5; // Linha original
    int col = index % 5; // Coluna original
    int row_corrigida = 4 - row; // Inverte a linha (0 -> 4, 1 -> 3, etc.)
    return row_corrigida * 5 + col; // Novo índice corrigido
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio1(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        int index_corrigido = corrigir_indice(i); // Corrige o índice do LED
        valor_led = matrix_rgb(desenho[index_corrigido] * r, desenho[index_corrigido] * g, desenho[index_corrigido] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função para exibir a animação da palavra "ALINE"
void animacao_ALINE(uint32_t valor_led, PIO pio, uint sm) {
    desenho_pio(letra_A, valor_led, pio, sm, 1.0, 0.0, 0.0); // Vermelho
    sleep_ms(1000);
    desenho_pio(letra_L, valor_led, pio, sm, 0.0, 1.0, 0.0); // Verde
    sleep_ms(1000);
    desenho_pio(letra_I, valor_led, pio, sm, 0.0, 0.0, 1.0); // Azul
    sleep_ms(1000);
    desenho_pio(letra_N, valor_led, pio, sm, 1.0, 1.0, 0.0); // Amarelo
    sleep_ms(1000);
    desenho_pio(letra_E, valor_led, pio, sm, 1.0, 0.0, 1.0); // Rosa
    sleep_ms(1000);
}

