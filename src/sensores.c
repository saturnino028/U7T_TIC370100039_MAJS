/**
 * @brief tratamento dos sensores externos
 */
#include "sensores.h"

/**
 * @brief função para verificar conexão dos sensores I2C
 * @return 1 se encontrou o endereço, 0 se falhou
 */

int verifica_sens(uint8_t _i2c_address)
{
    uint8_t *empty = 0;
    int verificador =  i2c_write_blocking(i2c0, _i2c_address, empty, 1, 0);

    return verificador;
}