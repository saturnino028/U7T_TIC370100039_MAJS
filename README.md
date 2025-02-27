# **Leito Inteligente – EmbarcaTech**

🚀 **Monitoramento avançado para pacientes acamados**

## 📌 **Sobre o Projeto**
O **Leito Inteligente** é um sistema embarcado projetado para **monitorar parâmetros vitais** de pessoas acamadas, 
auxiliando no cuidado e garantindo uma assistência mais rápida. Utiliza o **RP2040 (Raspberry Pi Pico W)** e sensores para medir 
**frequência cardíaca, saturação de oxigênio, temperatura corporal e umidade da fralda**, além de contar com um **botão de pânico** para emergências.

## 🛠️ **Hardware Utilizado**
- **Microcontrolador**: RP2040 (Raspberry Pi Pico W)
- **Sensores**:
  - MAX30102 – Oxímetro e batimentos cardíacos
  - AD8232 – ECG
  - MCP9808 – Sensor de temperatura
  - Sensor digital de umidade
- **Atuadores e Indicadores**:
  - Display **SSD1306** (I2C)
  - LEDs RGB WS2812
  - Buzzer
  - Botões físicos para emergências
- **Comunicação**:
  - **I2C** para sensores
  - **UART** para comunicação com o computador de borda
  - **(Futuro)** Wi-Fi/Bluetooth para integração IoT

## 📂 **Estrutura do Repositório**
```
📦 U7T_TIC370100039_MAJS
 ┣ 📂 src/                # Código-fonte
 ┣ 📂 inc/                # Arquivos de inclusão
 ┣ 📂 docs/               # Documentação
 ┣ 📜 README.md           # Este arquivo
 ┣ 📜 CMakeLists.txt      # Arquivo CMake
 ┣ 📜 InterfacePio.pio    # Assembly para uso do PIO
 ┗ 📜 .gitignore          # Arquivos ignorados pelo Git
```

## 🚀 **Instalação e Configuração**
1. **Clone este repositório**
   git clone https://github.com/saturnino028/U7T_TIC370100039_MAJS.git
2. **Instale o SDK do RP2040** ([Guia Oficial](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf))
3. **Import o projeto**
4. **Compile e envie o código**

## 📡 **Modo de Operação**
O sistema lê periodicamente os sensores e exibe os dados no **display OLED**, além de enviá-los para o computador via **UART**. 
O botão de pânico (botão A) pode ser acionado a qualquer momento para alertar uma emergência, assim como o botão da campainha (botão B)

## 🔄 **Futuras Melhorias**
✅ Integração com **Wi-Fi/Bluetooth** para monitoramento remoto  
✅ Implementação de **protocolos MQTT/HTTP** para integração IoT  
✅ Otimizações no consumo energético para uso com **bateria de backup**  

## 🤝 **Contribuição**
Sinta-se à vontade para abrir **issues** e enviar **pull requests** para melhorias!

## Demonstração

O vídeo demonstrando a organização e funcionamento pode ser acessado clicando [aqui](https://youtu.be/8vnq-YAyX94)

---
Desenvolvido para a capacitação em Sistemas Embarcados oferecida pela CEPEDI utilizando o Raspberry Pi Pico W, o Pico SDK e o kit de desenvolvimento BitDogLab.
