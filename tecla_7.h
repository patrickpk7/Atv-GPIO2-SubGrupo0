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


// Vetores para desenhos na matriz de LED
double desenho_1[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0};

double desenho_2[25] = {
    0.0, 0.0, 0.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double desenho_3[25] = {
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double desenho_4[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

double desenho_5[25] = {
    0.0, 1.0, 1.0, 1.0, 0.0,
    1.0, 0.0, 0.0, 0.0, 1.0,
    0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0
    };

// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb9(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 06) | (B << 8);
}

// Rotina para acionar a matriz de LEDs - WS2812B
void desenho_pio9(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb9(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void tecla_7 (uint32_t valor_led, PIO pio, uint sm){
            desenho_pio9(desenho_1, valor_led, pio, sm, 1.0, 0.647, 0.0);  // Laranja
            sleep_ms(1000);
            desenho_pio9(desenho_2, valor_led, pio, sm, 0.0, 1.0, 1.0);  // Ciano
            sleep_ms(1000);
            desenho_pio9(desenho_3, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Amarelo
            sleep_ms(1000);
            desenho_pio9(desenho_4, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
            sleep_ms(1000);
            desenho_pio9(desenho_5, valor_led, pio, sm, 0.5, 0.0, 0.5);  // Roxo
            sleep_ms(1000);

}
        

