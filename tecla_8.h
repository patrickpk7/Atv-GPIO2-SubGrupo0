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
double quadrado_preenchido[25] = {
    0.7, 0.7, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.7, 0.7, 0.7};

double cruz[25] = {
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

double letra_o[25] = {
    0.0, 0.5, 0.5, 0.5, 0.0,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.5, 0.0, 0.0, 0.0, 0.5,
    0.0, 0.5, 0.5, 0.5, 0.0};

double casa[25] = {
    1.0, 1.0, 1.0, 0.0, 1.0,
    1.0, 0.0, 1.0, 0.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0,
    0.0, 1.0, 1.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 0.0, 0.0};

double escada[25] = {
    0.7, 0.7, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.7, 0.7, 0.0,
    0.0, 0.0, 0.7, 0.7, 0.7,
    0.7, 0.7, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.7};

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

void aciona_tecla_8 (uint32_t valor_led, PIO pio, uint sm){
           desenho_pio(quadrado_preenchido, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
            sleep_ms(500);
            desenho_pio(cruz, valor_led, pio, sm, 0.0, 0.0, 1.0);  // Azul
            sleep_ms(500);
            desenho_pio(letra_o, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Amarelo
            sleep_ms(500);
            desenho_pio(casa, valor_led, pio, sm, 1.0, 0.0, 0.0);  // Vermelho
            sleep_ms(500);
            desenho_pio(escada, valor_led, pio, sm, 0.0, 1.0,1.0);  // Verde claro
            sleep_ms(500); 

}
        

