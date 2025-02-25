/**
 * @brief Arquivo Cabeçalho das Funções de Entrada/Saída Digital
 */
#ifndef FUNCOES_GPIO_H
#define FUNCOES_GPIO_H
/***********************  Includes ***********************/
#include <stdio.h>
#include "pinout.h"
#include "ssd1306.h"
#include "matriz_5x5.h"
#include "funcoes_pwm.h"
#include "pico/stdlib.h"
#include "figuras_ws2812.h"

/***********************  Defines ***********************/

/******************* Variáveis Globais *******************/

extern volatile uint8_t flag_botoes;

/******************* Protótipo Funções *******************/

void botoes_callback(uint gpio, uint32_t events);
void tratamento_bot(ssd1306_t *ssd);
void panico(ssd1306_t *ssd);
void botao_B(ssd1306_t *ssd);
void botao_joy(ssd1306_t *ssd);

void PontoLED(void);
void TracoLED(void);
void EspacoLED(void);
void sos_morse(void);

#endif //FUNCOES_GPIO_H