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
double frame1[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0
};

double frame2[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};

double frame3[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0
};

double frame4[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0
};

double frame5[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 0.0, 1.0, 1.0
};

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb5(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio14(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb5(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função para exibir a animação do coração pulsando em 5 cores
void animacao_CORACAO(uint32_t valor_led, PIO pio, uint sm) {
    

    // Exibe os frames em sequência.

      desenho_pio14(frame1, valor_led, pio, sm, 1.0, 0.0, 0.0); // Vermelho (100%)
      sleep_ms(1000); // Intervalo entre os frames.
      desenho_pio14(frame2, valor_led, pio, sm, 0.0, 1.0, 0.0); // Vermelho (80%)
      sleep_ms(1000); // Intervalo entre os frames.
      desenho_pio14(frame3, valor_led, pio, sm, 1.0, 0.0, 0.0); // Vermelho (60%)
      sleep_ms(1000); // Intervalo entre os frames.
      desenho_pio14(frame4, valor_led, pio, sm, 0.0, 0.0, 1.0); // Vermelho (40%)
      sleep_ms(1000); // Intervalo entre os frames.
      desenho_pio14(frame5, valor_led, pio, sm, 1.0, 0.0, 0.0); // Vermelho (20%)
      sleep_ms(1000); // Intervalo entre os frames.
        
       
    
}

