/**
 * @brief implementação das funções que utilizam controle PWM
 */
#include "funcoes_pwm.h"

/*********************** Declarações de variaveis **********************/

volatile uint8_t volume_buzA = 50; //Armazena o voulume do buzzer A
volatile uint8_t volume_buzB = 50; //Armazena o volume do buzzer B
volatile uint8_t altura_leito = 50; //Armazena a altura do leito
volatile uint8_t pos_cabeceira = 50; //Armazena a posicação da cabeceira

typedef struct 
{
    uint _slice;
    uint8_t _pin;
} def_canais_pwm;

def_canais_pwm dados;

/******************** Implementação das Funções de PWM ***************/
/**
 * @brief função de callback para desativar a campainha
 */
int64_t fim_campainha(alarm_id_t id, void *user_data)
{
    def_canais_pwm *data = (def_canais_pwm *)user_data;
    uint _slice = data -> _slice;
    uint8_t _pin = data -> _pin;
    duty_cicle(0.0, _slice, _pin);
    return 0;
}

/**
 * @brief ajusta o duty cicle
 */
void duty_cicle(float _percent, uint _slice, uint8_t _pin)
{
    pwm_set_enabled(_slice, false); //Desabilita PWM
    uint16_t valor_pwm = (_percent/100)*top_wrap; //Configura DutyCicle
    pwm_set_gpio_level(_pin, valor_pwm); //Configura DutyCicle
    pwm_set_enabled(_slice, true); //Habilitar PWM
}

/**
 * @brief função para som no buzzer
 */
void campainha(float _dc, uint32_t _duracao_ms, uint _slice, uint8_t _pin)
{
    duty_cicle(_dc, _slice, _pin);
    dados._slice = _slice;
    dados._pin = _pin;
    add_alarm_in_ms(_duracao_ms, fim_campainha, &dados, false);
}

void funcoes_joystick()
{
    
}