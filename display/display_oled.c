#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

// Bibliotecas para a matriz LED
#include <math.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"
#include "hardware/gpio.h"

// Arquivo .pio
#include "ws2818b.pio.h"

#define LED_PIN1 11
#define LED_PIN2 12
#define LED_PIN3 13
#define BUTTON_A 5
#define BUTTON_B 6

#define LED_COUNT 25
#define LED_PIN 7

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;


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
  
  // Função para converter a posição do matriz para uma posição do vetor.
  int getIndex(int x, int y) {
      // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
      // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
      if (y % 2 == 0) {
          return 24-(y * 5 + x); // Linha par (esquerda para direita).
      } else {
          return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
      }
  }

  void zero(){

    int matriz[5][5][3] = {
         {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    
    void one(){
    
    int matriz1[5][5][3] = {
         {{0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz1[coluna][linha][0], matriz1[coluna][linha][1], matriz1[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    
    void two(){
    
    int matriz2[5][5][3] = {
          {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz2[coluna][linha][0], matriz2[coluna][linha][1], matriz2[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    void tres(){
    
    int matriz3[5][5][3] = {
          {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz3[coluna][linha][0], matriz3[coluna][linha][1], matriz3[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    
    void four(){
    
    int matriz4[5][5][3] = {
         {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}}};
    
    


         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz4[coluna][linha][0], matriz4[coluna][linha][1], matriz4[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    
    void five(){
    
    int matriz5[5][5][3] = {
         {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz5[coluna][linha][0], matriz5[coluna][linha][1], matriz5[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    void six(){
    
    int matriz6[5][5][3] = {
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz6[coluna][linha][0], matriz6[coluna][linha][1], matriz6[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    void seven(){
    
    int matriz7[5][5][3] = {
         {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz7[coluna][linha][0], matriz7[coluna][linha][1], matriz7[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    
    void oito(){
    
    int matriz8[5][5][3] = {
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz8[coluna][linha][0], matriz8[coluna][linha][1], matriz8[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }
    
    void nine(){
    
    int matriz9[5][5][3] = {
         {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {11, 59, 244}, {0, 0, 0}},
        {{0, 0, 0}, {11, 59, 244}, {11, 59, 244}, {11, 59, 244}, {0, 0, 0}}};
    
    
         // Desenhando Sprite contido na matriz.c
      for(int linha = 0; linha < 5; linha++){
        for(int coluna = 0; coluna < 5; coluna++){
          int posicao = getIndex(linha, coluna);
          npSetLED(posicao, matriz9[coluna][linha][0], matriz9[coluna][linha][1], matriz9[coluna][linha][2]);
        }
      }
    
        npWrite();
        sleep_ms(2000);
        npClear();
        
    
    }


// Função de tratamento da interrupção (ISR)
void gpio_callback(uint gpio, uint32_t events) {
  if (gpio == BUTTON_A && (events & GPIO_IRQ_EDGE_FALL)) {
      gpio_xor_mask(1u << LED_PIN2);  // Inverte o estado do LED1
  }
  if (gpio == BUTTON_B && (events & GPIO_IRQ_EDGE_FALL)) {
      gpio_xor_mask(1u << LED_PIN3);  // Inverte o estado do LED2
  }
}

int main()
{
    stdio_init_all();   // Inicializa os tipos stdio padrão presentes ligados ao binário

    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(LED_PIN2);
    gpio_set_dir(LED_PIN2, GPIO_OUT);

    gpio_init(LED_PIN3);
    gpio_set_dir(LED_PIN3, GPIO_OUT);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    //gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
   //  gpio_pull_up(BUTTON_B);

   gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);
  // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();

  // Aqui, você desenha nos LEDs.

     npWrite(); // Escreve os dados nos LEDs.


    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
// /**
    char *text[] = {
        "  Oi   ",
        "    "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
// */

// Parte do código para exibir a linha no display (algoritmo de Bresenham)

    ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
    render_on_display(ssd, &frame_area);

    char c;
    int in = 1;



 

    while(true) {

      printf("Digite um caracter: ");
      scanf(" %c", &c);
      sleep_ms(1000);
      

     /* while(in % 2 == 0) {
        if ((gpio_get(BUTTON_A) == 0)) { //Pressionado  
    
          gpio_xor_mask(1u << LED_PIN2);
          printf("O estado do led foi mudado\n");  // Alterna o estado do LED
          sleep_ms(500); 
               // Atraso de 500 ms
          in ++;
          sleep_ms(500);
           };
      
        if ((gpio_get(BUTTON_B) == 0)) { //Pressionado  
    
          gpio_xor_mask(1u << LED_PIN3);
          printf("O estado do led foi mudado\n");  // Alterna o estado do LED
          sleep_ms(500);     
          in ++;    
          sleep_ms(500);        // Atraso de 500 ms
           };
    
    
        }*/


     

    if(c == 'A'){
            char *text[] = {
                "  LETRA:    ",
                "   A "};
        

            int y = 0;
            for (uint i = 0; i < count_of(text); i++)
            {
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
        // */
        
        // Parte do código para exibir a linha no display (algoritmo de Bresenham)
        
            ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
            render_on_display(ssd, &frame_area);
            sleep_ms(250);
            
            in ++;
        };

    if(c == 'B'){
            char *text[] = {
                "  LETRA:    ",
                "   B "};
        
            int y = 0;
            for (uint i = 0; i < count_of(text); i++)
            {
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
        // */
        
        // Parte do código para exibir a linha no display (algoritmo de Bresenham)
        
            ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
            render_on_display(ssd, &frame_area);
            
        };

    if(c == 'C'){
            char *text[] = {
                "  LETRA:    ",
                "   C "};
        
            int y = 0;
            for (uint i = 0; i < count_of(text); i++)
            {
                ssd1306_draw_string(ssd, 5, y, text[i]);
                y += 8;
            }
            render_on_display(ssd, &frame_area);
        // */
        
        // Parte do código para exibir a linha no display (algoritmo de Bresenham)
        
            ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
            render_on_display(ssd, &frame_area);
            
        };


    if(c == '0'){

            zero();
            
        }

    if(c == '1'){

            one();
            
        }

    if(c == '2'){

            two();
            
        }


        sleep_ms(400);
    }

    return 0;
}
