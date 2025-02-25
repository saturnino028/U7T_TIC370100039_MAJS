/**
 * @brief arquivo fonte funções da uart
 */
#include "uart.h"
/**
 * @brief Realiza alguma ação com base em algum caracter
 */
void acoes_char(int _char)
{
    switch (_char)
    {
        case '*':
            modo_gravacao();
            break;
        default:
            break;
    }
}

/**
 * Coloca o Pico no modo gravação
 */
void modo_gravacao()
{    
    printf("Entrando no modo de gravacao...\n");
    reset_usb_boot(0, 0); 
}