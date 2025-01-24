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

//botão de interrupção
const uint button_0 = 5;
const uint button_1 = 6;

//vetores para desenhos na matriz de LED
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

//imprimir valor binário
void imprimir_binario(int num) {
    for (int i = 31; i >= 0; i--) {
        (num & (1 << i)) ? printf("1") : printf("0");
    }
    printf("\n");
}

//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events) {
    printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
    printf("HABILITANDO O MODO GRAVAÇÃO");
    reset_usb_boot(0, 0); // habilita o modo de gravação do microcontrolador
}

//rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

//rotina para acionar a matriz de leds - ws2812b
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
}

//função principal
int main() {
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    double r = 1.0, g = 1.0, b = 1.0; // cores padrão (branco)

    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();

    printf("iniciando a transmissão PIO\n");
    if (ok)
        printf("clock set to %ld\n", clock_get_hz(clk_sys));

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN);
    gpio_pull_up(button_0);

    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN);
    gpio_pull_up(button_1);

    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);

    // Vetor com os desenhos
    double *desenhos[5] = {seta_baixo, seta_cima, letra_x, quadrado, losango};
    int desenho_atual = 0; // Índice do desenho atual

    while (true) {
        if (gpio_get(button_1)) {
            // Exibe o desenho atual
            desenho_pio(desenhos[desenho_atual], valor_led, pio, sm, r, g, b);

            // Avança para o próximo desenho
            desenho_atual = (desenho_atual + 1) % 5;

            // Pausa para que cada desenho seja visível antes de alternar
            sleep_ms(500);
        }
    }
}

