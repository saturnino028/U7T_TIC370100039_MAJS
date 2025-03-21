/**
 * @brief arquivo cabeçalho das funções de configuração
 */
#ifndef CONFIG_H
#define CONFIG_H
/***********************  Includes ***********************/
#include <stdio.h>
#include "pinout.h"
#include "ssd1306.h"
#include "funcoes_pwm.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"

/***********************  Defines ***********************/

#define KHz         1000 //Define KHz
#define MHz         1000000 //Define MHz

/******************* Variáveis Globais *******************/

extern volatile uint16_t top_wrap; //Maior valor de wrap;
extern volatile bool erro_flag ; //Sinal vizual de erro

/******************* Protótipo Funções *******************/

void config_adc();
uint config_pwm(uint8_t _pin, uint16_t _freq_Hz);
void config_spi();
void config_i2c();
void config_pins_gpio();

void calibrar_joy(ssd1306_t *ssd, uint16_t posicoesjoy[6]);

char mensagens_erro(ssd1306_t *ssd, uint8_t _chk_erro);
void erro_clock(ssd1306_t *ssd);
void erro_stdio(ssd1306_t *ssd);
void erro_display();
void erro_oximetro(ssd1306_t *ssd);
void erro_sen_temp(ssd1306_t *ssd);


#endif //CONFIG_H