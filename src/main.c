#include "main.h"



int main()
{
    //Declaração de variáveis
    ssd1306_t ssd; // Inicializa a estrutura do display
    bool cor = true; //Cor do display
    uint16_t valores_ref_joy[6] = {2047,4095,0,2047,4095,0}; /* é um vetor com as posições 
    * de calibração do joystick, nas três primeiras posiçõesestão armazenados os valores de 
    * referencia de X - X0, Xmax, Xmin. Nas três últimas, estão os valore de referencia de 
    * y - y0, ymax, ymin.
    */
    char buffer[4], buffer_erro[11]; //Buffer para string e para código de erro
    buffer_erro[5] = '\0';
    uint16_t valor_adc, valor_adc_passado = 0; //Armazena o valor lido pelo conversor AD
    absolute_time_t tempo_de_amostragem; //Coordena a taxa de amostragem do MIC
    const uint8_t amostras_por_segundo = 4; // Frequência de amostragem
    uint16_t adc_value_x, adc_value_y; //Auxiliar para leitura ADC do Joystick
    uint16_t passadoX, passadoY; //Armazena o valor antigo de X e Y
    uint64_t tempo_joy = 0;

    //Inicialização de hardware e software
    set_sys_clock_khz(1250000,false); //Cofigura o clock
    stdio_init_all();
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

    calibrar_joy(&ssd, valores_ref_joy); //Captura valores de referência do joystick
    
    gpio_put(LED_R, erro_flag);
    gpio_put(LED_G, !erro_flag);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Estamos Prontos", 3, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Para te ajudar", 8, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "BEM VINDO", 24, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(3000);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Funcionando", 3, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "de forma ", 3, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "Incompleta", 3, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(3000);

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, "Por favor", 8, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Consulte o", 8, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "manual", 8, 48); // Desenha uma string      
    ssd1306_send_data(&ssd); // Atualiza o display
    sleep_ms(2000);    
    
    tela_principal(&ssd, buffer_erro);

    //Ativação das interrupções
    gpio_set_irq_enabled_with_callback(bot_A, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão A
    gpio_set_irq_enabled_with_callback(bot_B, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão B
    gpio_set_irq_enabled_with_callback(bot_joy, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão Joystick

    tempo_de_amostragem = delayed_by_us(get_absolute_time(), 1000000/amostras_por_segundo); 
    
    //Loop Principal
    while(1)
    {
        int c = getchar_timeout_us(1000); //Fazer leitura da serial
        if(c != '\0' && c > 0)
            acoes_char(c); //Processa dados da serial
            
        tratamento_bot(&ssd); //Processa dados dos botões
        if(flag_botoes == 3) //Voltar para tela principal
        {
            tela_principal(&ssd, buffer_erro);

            ssd1306_draw_string(&ssd, buffer, 8, 48);
            ssd1306_send_data(&ssd);
            flag_botoes = 0;
        }
        
        //Sensor de Batimentos
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
            tempo_de_amostragem = delayed_by_us(get_absolute_time(), 1000000/amostras_por_segundo);
        }

        if((to_us_since_boot(get_absolute_time()) - tempo_joy) > 2000000) //deve se segurar o joy por 2 segundos para salvar posição
        {
            //Controle PWM
            adc_select_input(0); // Seleciona o ADC para eixo Y. O pino 26 como entrada analógica
            adc_value_y = adc_read();
            adc_select_input(1); // Seleciona o ADC para eixo X. O pino 27 como entrada analógica
            adc_value_x = adc_read();

            //Verifica se houve alteração significativa no eixo X (>5%)
            if(abs(passadoX - adc_value_x) > (passadoX*0.05)) 
            {
                passadoX = adc_value_x; //Atualiza variavel de controle
                funcoes_joystick(&ssd, passadoX, passadoY, valores_ref_joy);
                ssd1306_draw_string(&ssd, buffer, 8, 48);
                tela_principal(&ssd, buffer_erro);

            }
            //Verifica se houve alteração significativa no eixo Y (>5%)
            if(abs(passadoY - adc_value_y) > (passadoY*0.05))
            {
                passadoY = adc_value_y;//Atualiza variavel de controle
                funcoes_joystick(&ssd, passadoX, passadoY, valores_ref_joy);
                ssd1306_draw_string(&ssd, buffer, 8, 48);
                tela_principal(&ssd, buffer_erro);
            }
            tempo_joy = to_us_since_boot(get_absolute_time());
        }
    }
}