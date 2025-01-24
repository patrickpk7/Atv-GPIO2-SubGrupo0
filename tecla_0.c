#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"


//arquivo .pio
#include "ws2818b.pio.h"

//numero de LEDs
#define LED_COUNT 25

//pino de saída
#define LED_PIN 7

//Teclado matricial
uint linhas[4] = {1,2,3,4}; // Pinos das linhas do teclado matricial
uint colunas[4] = {5,6,7,8}; // Pinos das colunas do teclado matricial


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

// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}



int main() {

  // Inicializa entradas e saídas.
  stdio_init_all();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();

  // Aqui, você desenha nos LEDs.


  npWrite(); // Escreve os dados nos LEDs.

  // Não faz mais nada. Loop infinito.
  while (true) {
    sleep_ms(1000);
  }
}