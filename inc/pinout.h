/**
 * @brief Arquivo com a configuração dos pinos
 */
#ifndef PINOUT_H
#define PINOUT_H

/***********************  Includes ***********************/
#include <stdio.h>
#include "pico/stdlib.h"

/***********************  Defines ***********************/

#define uart_tx     0 //Uart 0 - TX
#define uart_rx     1 //Uart 0 - RX
#define pwm_alt     2 //Controle de altura do leito
#define pwm_aju     3 //Ajuste da posição da cabeceira
#define umi_pin     4 //Sensor de umidade
#define bot_A       5 //Botão do pânico
#define bot_B       6 //Campainha
#define mat_pin     7 //Matriz de LEDs
#define I2C0_SDA    8 //Sensores I2C
#define I2C0_SCL    9 //Sensores I2C
#define buz_B       10 //Buzzer B
#define LED_G       11 //LED verde
#define LED_B       12 //LED azul
#define LED_R       13 //LED vermelho
#define I2C1_SDA    14 //SDA Display
#define I2C1_SCL    15 //SCL Display
#define SPI_RX      16 //MISO
#define SPI_CS      17 //Seleciona o periferico
#define SPI_SCK     18 //Clock SPI
#define SPI_TX      19 //MOSI
#define buz_A       21 //Buzzer A
#define bot_joy     22 //Botão Joystick
#define adc_jY      26 //Eixo Y do Joystick
#define adc_jX      27 //Eixo X do Joystick
#define adc_mic     28 //Entrada analogica ESG ou MIC

/******************* Variáveis Globais *******************/

/******************* Protótipo Funções *******************/

#endif //PINOUT_H