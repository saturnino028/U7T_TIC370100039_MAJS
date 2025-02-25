/**
 * @brief cabeçalho das funções PWM
 */
#ifndef FUNCOES_PWM_H
#define FUNCOES_PWM_H
/***********************  Includes ***********************/
#include <stdio.h>
#include "config.h"
#include "pinout.h"
#include "ssd1306.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

/***********************  Defines ***********************/

/******************* Variáveis Globais *******************/

extern volatile uint8_t volume_buzA; //Armazena o voulume do buzzer A
extern volatile uint8_t volume_buzB; //Armazena o volume do buzzer B
extern volatile uint8_t altura_leito; //Armazena a altura do leito
extern volatile uint8_t pos_cabeceira; //Armazena a posicação da cabeceira

/******************* Protótipo Funções *******************/

void duty_cicle(float _percent, uint _slice, uint8_t _pin);
void campainha(float _dc,  uint32_t _duracao_ms, uint _slice, uint8_t _pin);
void controle_altura();
void controle_pos_cabeceira();

#endif //FUNCOES_PWM_H