/**
 * @brief arquivo de cabeçalho das funções de controle de sensores
 */
#ifndef SENSORES_H
#define SENSORES_H
/***********************  Includes ***********************/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

/***********************  Defines ***********************/

#define ADDR_oxim 0x57 //Endereço I2C do oximetro (padrão MAX30102)
#define ADDR_temp 0x18 //Endereço I2C do termomêtro (padrão MCP9808)

/******************* Variáveis Globais *******************/

/******************* Protótipo Funções *******************/

int verifica_sens(uint8_t _i2c_address);

#endif //SENSORES_H