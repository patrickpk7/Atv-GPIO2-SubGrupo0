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

void aciona_tecla_0 (uint32_t valor_led, PIO pio, uint sm){
            desenho_pio(seta_baixo, valor_led, pio, sm, 1.0, 0.0, 0.0);  // Vermelho
            sleep_ms(500);
            desenho_pio(seta_cima, valor_led, pio, sm, 0.0, 0.0, 1.0);  // Azul
            sleep_ms(500);
            desenho_pio(letra_x, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
            sleep_ms(500);
            desenho_pio(quadrado, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Amarelo
            sleep_ms(500);
            desenho_pio(losango, valor_led, pio, sm, 1.0,0.0,1.0);  // Rosa
            sleep_ms(500);

}
        
