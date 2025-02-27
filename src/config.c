/**
 * @brief implementa as funções do joystick
 */
#include "config.h"

/************************ Declaração de Variaveis ************************/

volatile uint16_t top_wrap = 1000; volatile bool erro_flag = 0;

/*********************** Implementação das funções **********************/
/**
 * @brief Configura os pinos ADC
 */
void config_adc()
{
    adc_init();
    adc_gpio_init(adc_jY);//Inicia pino ADC eixo y
    adc_gpio_init(adc_jX);//Inicia pino ADC eixo x
    adc_gpio_init(adc_mic); //Inicializa pino ADC com mic conectado
}

/**
 * @brief Configura os pinos PWM
 */
uint config_pwm(uint8_t _pin, uint16_t _freq_Hz)
{
    uint slice; float Fpwm;
    top_wrap = 1000000/_freq_Hz;
    gpio_set_function(_pin, GPIO_FUNC_PWM); //Habilita a função PWM
    slice = pwm_gpio_to_slice_num(_pin);//Obter o valor do slice correspondente ao pino
    pwm_set_clkdiv(slice, 125.0); //Define o divisor de clock
    pwm_set_wrap(slice, top_wrap); //Define valor do wrap
    Fpwm = 125000000/(125.0*top_wrap);
    printf("PWM definido para %.2f Hz\n", Fpwm);
    return slice; //Retorna o slice correspondente
}

/**
 * @brief configura os pinos SPI
 */
void config_spi()
{
    spi_init(spi0, 5*MHz); //Configura SPI para 5 MHz
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI); //Comfigura o pino do clock
    gpio_set_function(SPI_TX, GPIO_FUNC_SPI); //Configura o MOSI
    gpio_set_function(SPI_RX, GPIO_FUNC_SPI); //Configura o MISO
    gpio_set_function(SPI_CS, GPIO_FUNC_SPI); //Configura o CS
}

/**
 * @brief configura o I2C
 */
void config_i2c()
{
    i2c_init(i2c0, 400*KHz); //Define a velocidade do barramento para 400KHz 
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C); //Configura SDA
    gpio_pull_up(I2C0_SDA); //Configura resistor de pull up
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C); //Configura SCL
    gpio_pull_up(I2C0_SCL); //Configura resistor de pull up
}

/**
 * @brief inicia os pinos de GPIO
 */
void config_pins_gpio()
{
    //Configuração do botao A
    gpio_init(bot_A);
    gpio_pull_up(bot_A);
    gpio_set_dir(bot_A, GPIO_IN);

    //Configuração do botao B
    gpio_init(bot_B);
    gpio_pull_up(bot_B);
    gpio_set_dir(bot_B, GPIO_IN);

    //Configuração do botao do Joystick
    gpio_init(bot_joy);
    gpio_pull_up(bot_joy);
    gpio_set_dir(bot_joy, GPIO_IN);

    //Configuração da entrada sensor de umidade
    gpio_init(umi_pin);
    gpio_pull_up(umi_pin);
    gpio_set_dir(umi_pin, GPIO_IN);

    //Configuração do Buzzer B
    gpio_init(buz_A);
    gpio_set_dir(buz_A, GPIO_OUT);

    //Configuração do Buzzer B
    gpio_init(buz_B);
    gpio_set_dir(buz_B, GPIO_OUT);

    //Configuração do LED vermelho
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    //Configuração do LED verde
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    //Configuração do LED azul
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
}

/**
 * @brief função usada para calibrar a posição do Joystick
 * @param ssd ponteiro para uso do display
 * @param posicoesjoy é um vetor com as posições de calibração do joystick, nas três primeiras posições
 * estão armazenados os valores de referencia de X - X0, Xmax, Xmin. Nas três últimas, estão os valores
 * de referencia de y - y0, ymax, ymin.
 */
void calibrar_joy(ssd1306_t *ssd, uint16_t posicoesjoy[6])
{
    bool cor = true; uint32_t aux1 = 0, aux2 = 0, delay_padrao = 2500;
    uint8_t slice_b = config_pwm(buz_B, 1*KHz); //Configura um slice para 1KHz
    float duty_cicle = 50.0;


    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Primeiro, ", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Vamos Calibrar", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "O Joystick", 8, 48); // Desenha uma string      
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(3000);
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Siga todas", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "As instrucoes", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "A seguir", 8, 48); // Desenha uma string      
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(3000);
    campainha(duty_cicle, 500,slice_b, buz_B);
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Nao Mexa", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Fazendo a cali", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "bracao inicial", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(3000);

    for(int i = 10; i<100; i++) //Coleta 100 amostras da pos X0 e Y0 (1segundo de amostra)
    {
        adc_select_input(0); //Seleciona o canal em que o eixo y esta conectado GPIO 26
        aux2 = adc_read() + aux2;
        sleep_ms(5);

        adc_select_input(1); //Seleciona o canal em que o eixo x esta conectado GPIO 27
        aux1 = adc_read() + aux1;
        sleep_ms(5);
    }
    posicoesjoy[0] = aux1/100; //Média da posição inicial de x
    posicoesjoy[3] = aux2/100; //Média da posição inicial de y
    aux1 = aux2 = 0;
    
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Segure o Joy", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Todo a direita", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Espere", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao);  
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Calibrando", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Segure firme", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Por favor", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao); 
    for(int i = 0; i<200;i++) //Coleta 200 amostras da pos X (1segundo de amostra)
    {
        adc_select_input(1); //Seleciona o canal em que o eixo x esta conectado GPIO 27
        aux1 = adc_read() + aux1;
        sleep_ms(5);
    }
    aux1 = aux1/200;
    if(aux1>posicoesjoy[0])
        posicoesjoy[1] = aux1;
    else if(aux1 < posicoesjoy[0])
        posicoesjoy[2] = aux1;
    aux1 = 0;
    
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Segure o Joy", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Todo a esquerda", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Espere", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao);   
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Calibrando", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Segure firme", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Por favor", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao); 
    for(int i = 0; i<200;i++) //Coleta 200 amostras da pos X (1segundo de amostra)
    {
        adc_select_input(1); //Seleciona o canal em que o eixo x esta conectado GPIO 27
        aux1 = adc_read() + aux1;
        sleep_ms(5);
    }
    aux1 = aux1/200;
    if(aux1>posicoesjoy[0])
        posicoesjoy[1] = aux1;
    else if(aux1 < posicoesjoy[0])
        posicoesjoy[2] = aux1;
    aux1 = 0;
    
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Segure o Joy", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Todo para cima", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Espere", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao);    
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Calibrando", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Segure firme", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Por favor", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao); 
    for(int i = 0; i<200;i++) //Coleta 200 amostras da pos y (1segundo de amostra)
    {
        adc_select_input(0); //Seleciona o canal em que o eixo y esta conectado GPIO 26
        aux1 = adc_read() + aux1;
        sleep_ms(5);
    }
    aux1 = aux1/200;
    if(aux1>posicoesjoy[3])
        posicoesjoy[4] = aux1;
    else if(aux1 < posicoesjoy[0])
        posicoesjoy[5] = aux1;
    aux1 = 0;
    
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Segure o Joy", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Todo pra baixo", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Espere", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao);    
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "Calibrando", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Segure firme", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Por favor", 8, 48); // Desenha uma string   
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(delay_padrao); 
    for(int i = 0; i<200;i++) //Coleta 200 amostras da pos y (1segundo de amostra)
    {
        adc_select_input(0); //Seleciona o canal em que o eixo y esta conectado GPIO 26
        aux1 = adc_read() + aux1;
        sleep_ms(5);
    }
    aux1 = aux1/200;
    if(aux1>posicoesjoy[3])
        posicoesjoy[4] = aux1;
    else if(aux1 < posicoesjoy[0])
        posicoesjoy[5] = aux1;
    aux1 = 0;

    
    gpio_put(LED_G, 1);
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    gpio_put(LED_G, 0);
    sleep_ms(200);
    gpio_put(LED_G, 1);
    
    campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
    gpio_put(LED_G, 0);
    sleep_ms(200);
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(ssd, "O seu modulo", 8, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Esta pronto", 8, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Para ser usado.", 8, 48); // Desenha uma string      
    ssd1306_send_data(ssd); // Atualiza o display
    gpio_put(LED_G, 1);
    sleep_ms(delay_padrao);
}

char mensagens_erro(ssd1306_t *ssd, uint8_t _chk_erro)
{
    char erro = 'E';
    uint8_t slice_b = config_pwm(buz_B, 1*KHz); //Configura um slice para 1KHz
    float duty_cicle = 50.0;

    switch (_chk_erro)
    {
        case 1:
            erro_clock(ssd);
            campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
            erro = 'C';
            break;
        case 2:
            erro_stdio(ssd);
            campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
            erro = 'S';
            break;
        case 3:
            erro_display();
            campainha(duty_cicle, 500,slice_b, buz_B); //Alerta sonoro
            erro = 'D';
            break;
        case 4:
            erro_oximetro(ssd);
            campainha(duty_cicle, 1000,slice_b, buz_B); //Alerta sonoro
            erro  = 'O';
            break;
        case 5:
            erro_sen_temp(ssd);
            campainha(duty_cicle, 1000,slice_b, buz_B); //Alerta sonoro
            erro = 'T';
            break;
        
        default:
            erro = 'E';
            break;
    }
    erro_flag = 1;
    return erro;
}

/**
 * @brief função de debbug erro de clock
 */
void erro_clock(ssd1306_t *ssd)
{
    printf("Erro na configuração do clock do sistema\n");
    printf("Clock definido para %ld\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
}

/**
 * @brief função de debbug erro da biblioteca stdio
 */
void erro_stdio(ssd1306_t *ssd)
{
    gpio_put(LED_R, 1);
}

/**
 * @brief função de debbug erro do display
 */
void erro_display()
{
    gpio_put(LED_R, 1);

}

/**
 * @brief função de debbug erro oximetro
 */
void erro_oximetro(ssd1306_t *ssd)
{
    printf("Módulo Oximetro não encontrado\n");

    bool cor = true;

    gpio_put(LED_R, cor);

    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_draw_string(ssd, "Modulo Oximetro", 2, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Nao encontrado", 2, 30); // Desenha uma string  
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(3000);

}

/**
 * @brief função de debbug erro sensor de temperatura
 */
void erro_sen_temp(ssd1306_t *ssd)
{

    printf("Módulo Sensor de Temperatura não encontrado\n");

    bool cor = true;

    gpio_put(LED_R, cor);
    
    ssd1306_fill(ssd, !cor); // Limpa o display
    ssd1306_draw_string(ssd, "Sensor de", 3, 10); // Desenha uma string
    ssd1306_draw_string(ssd, "Temperatura", 3, 30); // Desenha uma string
    ssd1306_draw_string(ssd, "Nao Encontrado", 3, 48); // Desenha uma string      
    ssd1306_send_data(ssd); // Atualiza o display
    sleep_ms(3000);
}