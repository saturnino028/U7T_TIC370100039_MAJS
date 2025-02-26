#include "main.h"



int main()
{
    //Declaração de variáveis
    uint slice_alt, slice_aju; //Variaveis para armazenar o slice da altura e do ajuste
    ssd1306_t ssd; // Inicializa a estrutura do display
    bool cor = true; //Cor do display
    uint16_t valores_ref_joy[6] = {2047,4095,0,2047,4095,0}; /* é um vetor com as posições 
    * de calibração do joystick, nas três primeiras posiçõesestão armazenados os valores de 
    * referencia de X - X0, Xmax, Xmin. Nas três últimas, estão os valore de referencia de 
    * y - y0, ymax, ymin.
    */
    char buffer[4], buffer_erro[6]; //Buffer para string e para código de erro
    buffer_erro[5] = '\0';
    uint16_t valor_adc, valor_adc_passado; //Armazena o valor lido pelo conversor AD

    //Inicialização de hardware e software
    set_sys_clock_khz(1250000,false); //Cofigura o clock
    stdio_init_all();
    slice_alt = config_pwm(pwm_alt, 1*KHz); //Configura PWM controle de altura do leito
    slice_aju = config_pwm(pwm_aju, 1*KHz); //Configura PWM controle de posição cabeceira
    config_pins_gpio(); //Configura os pinos GPIO
    init_matriz(); //Configura a matriz de LEDs 5x5 WS2812
    config_i2c(); //Configura I2C para os sensores
    config_i2c_display(&ssd); //Configura o I2C para o diplay
    config_spi(); //Configura o SPI para memoria
    config_adc(); //Configura canais ADC

    //Mensagens de boas vindas e teste de perifericos
    desenhar_fig(open, brilho_matriz); //Matriz RGB sorrindo

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_image(&ssd, Slogan);
    sleep_ms(3000);

    desenhar_fig(apagado, brilho_matriz); //Apaga Matriz RGB
    
    if(verifica_sens(ADDR_oxim) < 0)
        buffer_erro[3] = mensagens_erro(&ssd, 4);   

    if(verifica_sens(ADDR_temp) < 0)
        buffer_erro[4] = mensagens_erro(&ssd, 5);  

    //calibrar_joy(&ssd, valores_ref_joy); //Captura valores de referência do joystick
    
    gpio_put(LED_R, erro_flag);
    gpio_put(LED_G, !erro_flag);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_string(&ssd, "Estamos Prontos", 3, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Para te ajudar", 8, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "BEM VINDO", 24, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(3000);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_string(&ssd, "Funcionando", 3, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "de forma ", 3, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "Incompleta", 3, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(3000);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_string(&ssd, "Por favor", 8, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Consulte o", 8, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "manual", 8, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(2000);    
    
    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_image(&ssd, fig_principal); //Tela Principal
    ssd1306_draw_string(&ssd, "TEC SAUDE", 30, 8); // Desenha uma string
    ssd1306_line(&ssd, 26,21,106,21,cor);
    ssd1306_rect(&ssd, 0, 0, 128, 64, cor, !cor); // Desenha um retângulo
    ssd1306_line(&ssd, 0,26,128,26,cor); //Linha horizontal
    ssd1306_line(&ssd, 43,27,43,63, cor); //Linha Vertical
    ssd1306_line(&ssd, 86,27,86,63, cor); //Linha Vertical
    if(buffer_erro[3] == 'O')
        ssd1306_draw_string(&ssd, "OUT", 95, 48);
    if(buffer_erro[4] == 'T')
            ssd1306_draw_string(&ssd, "OUT", 55, 48);
    ssd1306_send_data(&ssd);  // Atualiza a tela com os novos pixels

    //Ativação das interrupções
    gpio_set_irq_enabled_with_callback(bot_A, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão A
    gpio_set_irq_enabled_with_callback(bot_B, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão B
    gpio_set_irq_enabled_with_callback(bot_joy, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão Joystick

    valor_adc_passado = 0;

    absolute_time_t tempo_de_amostragem = delayed_by_us(get_absolute_time(), 1000000/8000);
    //Loop Principal
    while(1)
    {
        int c = getchar_timeout_us(1000); //Fazer leitura da serial
        if(c != '\0' && c > 0)
            acoes_char(c); //Processa dados da serial
        tratamento_bot(&ssd); //Processa dados dos botões
        if(time_reached(tempo_de_amostragem)) // Coleta amostras no mic
        {        
            adc_select_input(2); // Canal do MIC
            valor_adc = adc_read();

            // Verifica se houve alteração significativa no mic (>5%)
            if(abs(valor_adc - valor_adc_passado) > (valor_adc_passado * 0.05)) 
            {  
                uint8_t valor_convertido = (uint8_t)((valor_adc * 200.0) / 4096.0);
                
                //Limpar região do número
                memset(buffer, 0, sizeof(buffer));
                strcpy(buffer, "   ");  // Preenche com espaços (3 caracteres) 
                ssd1306_draw_string(&ssd, buffer, 8, 48); //Limpara número
                ssd1306_send_data(&ssd);

                // Atualiza a tela SOMENTE após a mudança
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%d", valor_convertido);  // Máximo de 200 BPM
                ssd1306_draw_string(&ssd, buffer, 8, 48);
                ssd1306_send_data(&ssd);
                
                valor_adc_passado = valor_adc;
            }
            tempo_de_amostragem = delayed_by_us(get_absolute_time(), 1000000/8000);
        }
    }
}
