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
   char buffer[5];
   uint16_t valor_adc;

    //Inicialização de software e hardware
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
    //calibrar_joy(&ssd, valores_ref_joy); //Captura valores de referência do joystick

    ssd1306_fill(&ssd, !cor); // Limpa o display
    ssd1306_draw_string(&ssd, "Estamos Prontos", 3, 10); // Desenha uma string
    ssd1306_draw_string(&ssd, "Para te ajudar", 8, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "BEM VINDO", 24, 48); // Desenha uma string      
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
    ssd1306_send_data(&ssd);  // Atualiza a tela com os novos pixels

    //Ativação das interrupções
    gpio_set_irq_enabled_with_callback(bot_A, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão A
    gpio_set_irq_enabled_with_callback(bot_B, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão B
    gpio_set_irq_enabled_with_callback(bot_joy, GPIO_IRQ_EDGE_FALL, true, botoes_callback); //Interrupção botão Joystick

    //Loop Principal
    while(1)
    {
        int c = getchar_timeout_us(1000); //Fazer leitura da serial
        if(c != '\0' && c > 0)
            acoes_char(c); //Processa dados da serial
        tratamento_bot(&ssd); //Processa dados dos botões
    }
}
