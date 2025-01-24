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

//Teclado matricial

uint linhas[4] = {1,2,3,4}; // Pinos das linhas do teclado matricial
uint colunas[4] = {5,6,8,9}; // Pinos das colunas do teclado matricial


///Matriz para mapear as teclas do teclado matricial 4x4 para os caracteres correspondentes a cada tecla pressionada (0-9, A-D, *, #)  
char MAPA_TECLAS[4][4] = { 
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};

//Função para inicializar os GPIOs do teclado matricial
void inicia_teclado() {
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_init(linhas[i]); //Inicializa o pino GPIO especificado no índice i do array linhas
        gpio_set_dir(linhas[i], GPIO_OUT); //Configuração do modo de operação (direção) do pino GPIO indicado por linhas[i] como uma saída digital
    }
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_init(colunas[i]); //Inicializa o pino GPIO especificado no índice i do array colunas
        gpio_set_dir(colunas[i], GPIO_IN); //Configuração do modo de operação (direção) do pino GPIO indicado por linhas[i] como uma entrada
        gpio_pull_down(colunas[i]); //Ativa resistor de pull-down, quando nenhuma tecla estiver pressionada, o valor lido no pino será "0" (baixo)
    } 
}

//Função para ler o teclado matricial (verifica se alguma tecla foi pressionada)
char ler_teclado(){ //Declara a função chamada ler_teclado que retorna um valor do tipo char, a tecla pressionada
    for (int i = 0; i < 4; i++){ //Repete o bloco de código para os valores de i começando de 0 até 3 (inclusive), incrementando i em 1 a cada iteração
        gpio_put(linhas[i], 1); //Ativa a linha ( valor de 1 (ou HIGH) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
        for (int j = 0; j < 4; j++){ //Repete o bloco de código para os valores de j começando de 0 até 3 (inclusive), incrementando j em 1 a cada iteração
    if (gpio_get(colunas[j])){ //Verifica se a coluna foi ativada
        gpio_put(linhas[i],0); //Desativa a linha (valor de 0 (ou LOW) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
        sleep_ms(50); // Debounce (Espera 50 milissegundos para garantir que o sinal seja estável, evitando leituras falsas)
        return MAPA_TECLAS[i][j]; //Retorna a tecla pressionada
    }
 }
 gpio_put(linhas[i],0); ////Desativa a linha (valor de 0 (ou LOW) está sendo enviado para o pino GPIO especificado no índice i da array linhas)
    }
    return '\0'; //Retorna '\0' se nenhuma tecla foi pressionada
}


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

    //Inicializa o teclado matricial
    inicia_teclado();
    

    // Vetor com os desenhos
    double *desenhos[5] = {seta_baixo, seta_cima, letra_x, quadrado, losango};
    

    while (true) {
       
       char tecla = ler_teclado(); //Lê a tecla pressionada

       if(tecla == '0'){ //Verifica se a tecla '0' foi pressionada
            for (int i = 0; i < 5; i++) {
                desenho_pio(desenhos[i], valor_led, pio, sm, r, g, b);
                sleep_ms(500); //Pausa entre os desenhos
            }
       }
    }
}

