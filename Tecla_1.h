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

// Definição dos frames da animação (5 frames).
double X_1[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0
};

double X_2[25] = {
    0.8, 0.0, 0.0, 0.0, 0.8,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.0, 0.0, 0.8, 0.0, 0.0,
    0.0, 0.8, 0.0, 0.8, 0.0,
    0.8, 0.0, 0.0, 0.0, 0.8
};

double X_3[25] = {
    0.6, 0.0, 0.0, 0.0, 0.6,
    0.0, 0.6, 0.0, 0.6, 0.0,
    0.0, 0.0, 0.6, 0.0, 0.0,
    0.0, 0.6, 0.0, 0.6, 0.0,
    0.6, 0.0, 0.0, 0.0, 0.6
};

double X_4[25] = {
    0.4, 0.0, 0.0, 0.0, 0.4,
    0.0, 0.4, 0.0, 0.4, 0.0,
    0.0, 0.0, 0.4, 0.0, 0.0,
    0.0, 0.4, 0.0, 0.4, 0.0,
    0.4, 0.0, 0.0, 0.0, 0.4
};

double X_5[25] = {
    0.2, 0.0, 0.0, 0.0, 0.2,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.0, 0.0, 0.2, 0.0, 0.0,
    0.0, 0.2, 0.0, 0.2, 0.0,
    0.2, 0.0, 0.0, 0.0, 0.2
};

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb10(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio10(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb10(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função para exibir a animação do "X" pulsando em 5 frames
void animacao_X(uint32_t valor_led, PIO pio, uint sm) {
    // Exibe os frames em sequência.
    desenho_pio10(X_1, valor_led, pio, sm, 1.0, 0.0, 0.0); // Vermelho (100%)
    sleep_ms(1000); // Intervalo entre os frames.
    desenho_pio10(X_2, valor_led, pio, sm, 0.0, 1.0, 0.0); // Verde (80%)
    sleep_ms(1000); // Intervalo entre os frames.
    desenho_pio10(X_3, valor_led, pio, sm, 0.0, 0.0, 1.0); // Azul (60%)
    sleep_ms(1000); // Intervalo entre os frames.
    desenho_pio10(X_4, valor_led, pio, sm, 1.0, 1.0, 0.0); // Amarelo (40%)
    sleep_ms(1000); // Intervalo entre os frames.
    desenho_pio10(X_5, valor_led, pio, sm, 1.0, 0.0, 1.0); // Rosa (20%)
    sleep_ms(1000); // Intervalo entre os frames.
}