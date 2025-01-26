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
#define OUT_PIN 0


// Vetores para desenhos na matriz de LED
double frame1[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double frame2[25] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double frame3[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double frame4[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double frame5[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 06) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void animacao (uint32_t valor_led, PIO pio, uint sm){
            desenho_pio(frame1, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Amarelo
            sleep_ms(1000);
            desenho_pio(frame2, valor_led, pio, sm, 1.0, 1.0, 1.0);  // Branco
            sleep_ms(1000);
            desenho_pio(frame3, valor_led, pio, sm, 0.0, 1.0, 1.0);  // Ciano
            sleep_ms(1000);
            desenho_pio(frame4, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
            sleep_ms(1000);
            desenho_pio(frame5, valor_led, pio, sm, 0.5, 0.0, 0.5);  // Roxo
            sleep_ms(1000);

}
        

