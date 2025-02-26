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

uint16_t funcoes_joystick(ssd1306_t *ssd, uint16_t _adc_value_x, uint16_t _adc_value_y, uint16_t _ref[6])
{

    switch (func_bot_joy)
    {
        case 0:
            ajuste_leito(ssd, _adc_value_x, _adc_value_y, _ref);
            break;
        case 1:
            ajuste_sinais(ssd, _adc_value_x, _adc_value_y, _ref);
            break;
        default:
            break;
    }
}

void ajuste_leito(ssd1306_t *ssd, uint16_t _adc_value_x, uint16_t _adc_value_y, uint16_t _ref[6])
{
    uint slice_alt, slice_aju; //Variaveis para armazenar o slice da altura e do ajuste
    slice_alt = config_pwm(pwm_alt, 1*KHz); //Configura PWM controle de altura do leito
    slice_aju = config_pwm(pwm_aju, 1*KHz); //Configura PWM controle de posição cabeceira

    char str_x[4];  // Buffer para armazenar a string
    char str_y[4];  // Buffer para armazenar a string
    uint8_t aux = 0;
    bool cor = true;
    if(_ref[0] != 0)
    {
        if(_adc_value_x < _ref[0])//Se estiver indo para esquerda
            aux = ((_ref[0] - _adc_value_x) * 100) / (_ref[0] - _ref[2]);//Logica PWM
        else if(_adc_value_x > _ref[0])//Se estiver indo para a direita
            aux = ((_adc_value_x - _ref[0]) * 100) / (_ref[1] - _ref[0]);//Logica PWM
        else
            aux = 0;

        sprintf(str_x, "%d", aux);  // Converte o inteiro em string
        if(aux<10)
            duty_cicle(0, slice_aju, pwm_aju);
        else
            duty_cicle(aux, slice_aju, pwm_aju);
    }
    if(_ref[3] != 0) //Evitar erros
    {
        //Logica PWM
        aux = 0;
        if(_adc_value_y < _ref[3])//Se estiver indo para baixo
            aux = ((_ref[3] - _adc_value_y) * 100) / (_ref[3] - _ref[5]);//Logica PWM
        else if(_adc_value_y > _ref[3])//Se estiver indo para cima
            aux = ((_adc_value_y - _ref[3]) * 100) / (_ref[4] - _ref[3]);//Logica PWM
        else
            aux = 0;
        sprintf(str_y, "%d", aux);  // Converte o inteiro em string
        if(aux<10)
            duty_cicle(0, slice_alt, pwm_alt);
        else
            duty_cicle(aux, slice_alt, pwm_alt);
    }
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
    ssd1306_line(ssd, 3, 16, 123, 16, cor); // Desenha uma linha
    ssd1306_line(ssd, 3, 37, 123, 37, cor); // Desenha uma linha   
    ssd1306_draw_string(ssd, "AJUSTE LEITO", 16, 6); // Desenha uma string
    ssd1306_draw_string(ssd, "C - Cabeceira", 8, 18); // Desenha uma string
    ssd1306_draw_string(ssd, "H - Altura", 8, 28); // Desenha uma string 
    ssd1306_draw_string(ssd, "C    H    ", 20, 41); // Desenha uma string
    ssd1306_line(ssd, 44, 37, 44, 60, cor); // Desenha uma linha vertical     
    ssd1306_draw_string(ssd, str_x, 8, 52); // Desenha uma string  
    ssd1306_line(ssd, 84, 37, 84, 60, cor); // Desenha uma linha vertical 
    ssd1306_draw_string(ssd, str_y, 49, 52); // Desenha uma string
    ssd1306_send_data(ssd);
    sleep_ms(2000);
}

void ajuste_sinais(ssd1306_t *ssd, uint16_t _adc_value_x, uint16_t _adc_value_y, uint16_t _ref[6])
{
    char str_x[4];  // Buffer para armazenar a string
    char str_y[4];  // Buffer para armazenar a string
    uint8_t aux = 0;
    bool cor = true;
    uint slice_Led_R = config_pwm(LED_R, 1*KHz);

    if(_ref[0] != 0)
    {
        if(_adc_value_x < _ref[0])//Se estiver indo para esquerda
            aux = ((_ref[0] - _adc_value_x) * 100) / (_ref[0] - _ref[2]);//Logica PWM
        else if(_adc_value_x > _ref[0])//Se estiver indo para a direita
            aux = ((_adc_value_x - _ref[0]) * 100) / (_ref[1] - _ref[0]);//Logica PWM
        else
            aux = 0;

        sprintf(str_x, "%d", aux);  // Converte o inteiro em string
        if(aux<10)
            duty_cicle(0, slice_Led_R, LED_R);
        else
            duty_cicle(aux, slice_Led_R, LED_R);

    }
    if(_ref[3] != 0) //Evitar erros
    {
        //Logica PWM
        aux = 0;
        if(_adc_value_y < _ref[3])//Se estiver indo para baixo
            aux = ((_ref[3] - _adc_value_y) * 100) / (_ref[3] - _ref[5]);//Logica PWM
        else if(_adc_value_y > _ref[3])//Se estiver indo para cima
            aux = ((_adc_value_y - _ref[3]) * 100) / (_ref[4] - _ref[3]);//Logica PWM
        else
            aux = 0;
        sprintf(str_y, "%d", aux);  // Converte o inteiro em string
    }
    // Atualiza o conteúdo do display com animações
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 60, cor, !cor); // Desenha um retângulo
    ssd1306_line(ssd, 3, 16, 123, 16, cor); // Desenha uma linha
    ssd1306_line(ssd, 3, 37, 123, 37, cor); // Desenha uma linha   
    ssd1306_draw_string(ssd, "AJUSTE SINAIS", 16, 6); // Desenha uma string
    ssd1306_draw_string(ssd, "V - Volume", 8, 18); // Desenha uma string
    ssd1306_draw_string(ssd, "L - Brilho", 8, 28); // Desenha uma string 
    ssd1306_draw_string(ssd, "V    L    ", 20, 41); // Desenha uma string
    ssd1306_line(ssd, 44, 37, 44, 60, cor); // Desenha uma linha vertical     
    ssd1306_draw_string(ssd, str_x, 8, 52); // Desenha uma string  
    ssd1306_line(ssd, 84, 37, 84, 60, cor); // Desenha uma linha vertical 
    ssd1306_draw_string(ssd, str_y, 49, 52); // Desenha uma string
    ssd1306_send_data(ssd);
    sleep_ms(2000);
}