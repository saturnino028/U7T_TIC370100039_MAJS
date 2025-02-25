/**
 * @brief Arquivo de cabeçalho comunicação serial UART
 */
#ifndef UART_H
#define UART_H
/***********************  Includes ***********************/
#include <stdio.h>
#include "pinout.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/uart.h"

/***********************  Defines ***********************/

/******************* Variáveis Globais *******************/

/******************* Protótipo Funções *******************/

void acoes_char(int _char);
void modo_gravacao();

#endif //UART_H