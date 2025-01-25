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


// Rotina para definir a intensidade de cores do LED
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para desligar todos os LEDs
void aciona_tecla_A (PIO pio, uint sm) {
    uint32_t valor_led = matrix_rgb(0.0, 0.0, 0.0);
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

