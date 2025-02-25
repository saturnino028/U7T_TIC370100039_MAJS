/**
 * @brief Arquivo de implementação das funções de entrada/saida digital
 */
#include "funcoes_gpio.h"
volatile uint32_t passado = 0; //Usada para implementar o debouncing
volatile uint8_t flag_botoes = 0; //Usada para sinalizar eventos de interrupção
volatile bool func_bot_joy = 0; //determina a função que será executada pelo Joystick
volatile bool modo_panico = 0;  //Ativa o modo pânico/emergência
volatile bool control = true;
volatile uint16_t freq = 1000, freq_min = 1000, freq_max = 1500;

/**
 * @brief trata a interrupção gerada pelos botões A e B da BitDog
 * @param gpio recebe o pino que gerou a interrupção
 * @param events recebe o evento que causou a interrupção
 */
void botoes_callback(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t agora = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (agora - passado > 500000) // 500 ms de debouncing
    {
        passado  = agora;
        if(gpio == bot_A)
        {
            modo_panico = !modo_panico;
            flag_botoes = 1;
        }
        else if(gpio == bot_B)
            flag_botoes = 2;
        else if(gpio == bot_joy)
            flag_botoes = 3;
    }
}

/**
 * @brief realiza as ações com base no botão
 */
void tratamento_bot(ssd1306_t *ssd)
{
    switch (flag_botoes)
    {
    case 1:
        panico(ssd);
        flag_botoes = 0;
        break;
    case 2:
        botao_B(ssd);
        flag_botoes = 0;
        break;
    case 3:
        botao_joy(ssd);
        flag_botoes = 0;
        break;
    default:
        break;
    }

}

/**
 * @brief ajustar frequencia da sirene
 */
bool AlteraFreqSirene(struct repeating_timer *t)
{
    uint slice_a = config_pwm(buz_A, freq);
    uint slice_b = config_pwm(buz_B, freq);
    float volume_panico = 50.0;        
    duty_cicle(volume_panico, slice_a, buz_A);
    duty_cicle(volume_panico, slice_b, buz_B);            
    if(freq <= freq_min)
        control = true;
    else if(freq >= freq_max)
        control = false;
    if(control)
        freq += 20;
    else
        freq -= 20;
    if(!modo_panico)
    {
        duty_cicle(0, slice_a, buz_A);
        duty_cicle(0, slice_b, buz_B);     
    }
    return modo_panico;
}

/**
 * @brief botão do pânico
 */
void panico(ssd1306_t *ssd)
{
    struct repeating_timer timer;
    add_repeating_timer_ms(50, AlteraFreqSirene, NULL, &timer);
    while(modo_panico)
    {
        sos_morse();
        if(!modo_panico)
            desenhar_fig(apagado, 50);
    }
}

/**
 * Tocar campainha interna
 */
void botao_B(ssd1306_t *ssd)
{
    uint slice_b = config_pwm(buz_B, 2*KHz);
    campainha(volume_buzB,1000, slice_b, buz_B);
    printf("Precisa de ajuda\n");
}

/**
 * @brief Muda Função do Joy
 */
void botao_joy(ssd1306_t *ssd)
{
    bool cor = true;

    func_bot_joy = !func_bot_joy;

    
    uint slice_a = config_pwm(buz_A, 1*KHz);
    campainha(volume_buzA,200, slice_a, buz_A);

    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 0, 0, 128, 64, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Funcao Bot Joy", 8, 30); // Desenha uma string
    if(!func_bot_joy)
        ssd1306_draw_string(ssd, "Ajuste Leito", 8, 48); // Desenha uma string      
    else
        ssd1306_draw_string(ssd, "Ajuste Sinais", 8, 48); // Desenha uma string      
        
    ssd1306_send_data(ssd); // Atualiza o display
}


/**
 * @brief codifica um ponto em sinal luminoso
 */
void PontoLED()
{
    //Um Ponto(.) é representado pelo LED aceso por 0,2 segundos.
    desenhar_fig(red_fig, brilho_matriz);
    sleep_ms(200);
    desenhar_fig(apagado, brilho_matriz);
    sleep_ms(125);
    printf(".");
}

/**
 * @brief codifica um traço em sinal luminoso
 */
void TracoLED()
{
    //Um traço(-) é representado pelo LED aceso por 0,8 segundos.
    desenhar_fig(red_fig, brilho_matriz);
    sleep_ms(800);
    desenhar_fig(apagado, brilho_matriz);
    sleep_ms(125);
    printf("-");
}

/**
 * @brief implementa espaço após cada letra com sinal luminoso (somado com 125 ms anterior é igual a 250 ms)
 */
void EspacoLED()
{
    //O intervalo entre letras deve ser de 0,25 segundos.
    sleep_ms(125);
}

/**
 * @brief sinalizar SOS na matriz 5x5
 */
void sos_morse()
{
    //...
    PontoLED();
    PontoLED();
    PontoLED();
    EspacoLED();

    //---
    TracoLED();
    TracoLED();
    TracoLED();
    EspacoLED();
    
    //...
    PontoLED();
    PontoLED();
    PontoLED();
    EspacoLED();
}