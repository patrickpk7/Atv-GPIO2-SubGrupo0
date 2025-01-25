#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

//arquivo .pio
#include "pio_matrix.pio.h"

//número de LEDs
#define NUM_PIXELS 25

//pino de saída
#define OUT_PIN 7



double desenho_tecla5[25] =   {0.0, 0.0, 0.0, 0.0, 0.0,
                               0.0, 0.0, 1.0, 0.0, 0.0, 
                               0.0, 1.0, 0.0, 1.0, 0.0,
                               0.0, 0.0, 1.0, 0.0, 0.0,
                               0.0, 0.0, 0.0, 0.0, 0.0};
double desenho2_tecla5[25] =   {0.0, 0.0, 0.0, 0.0, 0.0,
                                0.0, 1.0, 1.0, 1.0, 0.0, 
                                0.0, 1.0, 0.0, 1.0, 0.0,
                                0.0, 1.0, 1.0, 1.0, 0.0,
                                0.0, 0.0, 0.0, 0.0, 0.0};
double desenho3_tecla5[25] =   {0.0, 0.0, 1.0, 0.0, 0.0,
                                0.0, 1.0, 1.0, 1.0, 0.0, 
                                1.0, 1.0, 0.0, 1.0, 1.0,
                                0.0, 1.0, 1.0, 1.0, 0.0,
                                0.0, 0.0, 1.0, 0.0, 0.0};
double desenho4_tecla5[25] =   {0.0, 0.0, 1.0, 0.0, 0.0,
                                0.0, 1.0, 0.0, 1.0, 0.0, 
                                1.0, 0.0, 0.0, 0.0, 1.0,
                                0.0, 1.0, 0.0, 1.0, 0.0,
                                0.0, 0.0, 1.0, 0.0, 0.0};
double desenho5_tecla5[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                                1.0, 0.0, 0.0, 0.0, 1.0, 
                                1.0, 0.0, 0.0, 0.0, 1.0,
                                1.0, 0.0, 0.0, 0.0, 1.0,
                                1.0, 1.0, 1.0, 1.0, 1.0};
double desenho_tecla_hashtag[25] =   {0.2, 0.2, 0.2, 0.2, 0.2,
                                      0.2, 0.2, 0.2, 0.2, 0.2, 
                                      0.2, 0.2, 0.2, 0.2, 0.2,
                                      0.2, 0.2, 0.2, 0.2, 0.2,
                                      0.2, 0.2, 0.2, 0.2, 0.2};

uint32_t matrix_rgb7(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matrix de leds - ws2812b
void desenho_pio5(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb7(desenho[i]*r, desenho[i]*b, desenho[i]*g);
         pio_sm_put_blocking(pio, sm, valor_led);
    }
    
}

void funcao_tecla_5(){
    uint32_t valor_led;
    //configurações da PIO
    PIO pio = pio0; 
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    desenho_pio5(desenho_tecla5, valor_led, pio, sm, 1.0, 1.0, 1.0);  // Vermelho
    sleep_ms(1000);
    desenho_pio5(desenho2_tecla5, valor_led, pio, sm, 0.0, 0.0, 1.0);  // Azul
    sleep_ms(1000);
    desenho_pio5(desenho3_tecla5, valor_led, pio, sm, 0.0, 1.0, 0.0);  // Verde
    sleep_ms(1000);
    desenho_pio5(desenho4_tecla5, valor_led, pio, sm, 1.0, 1.0, 0.0);  // Amarelo
    sleep_ms(1000);
    desenho_pio5(desenho5_tecla5, valor_led, pio, sm, 1.0,0.0,1.0);  // Rosa
    sleep_ms(1000);
}

// Aciona a tecla # para ligar o led branco em 20% de intensidade
void funcao_tecla_hashtag(PIO pio, uint sm){ 
    uint32_t valor_led = matrix_rgb2(0.2, 0.2, 0.2);
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}