# 🎵🖥️ Música com Animação em Display OLED na Raspberry Pi Pico W

Este projeto utiliza a **Raspberry Pi Pico W** com o **Pico SDK** para tocar uma música e, simultaneamente, exibir um GIF animado em um display **OLED SSD1306** via I2C. O código foi desenvolvido em C e utiliza **ambos os núcleos do microcontrolador** RP2040: um núcleo cuida da reprodução da música, enquanto o outro atualiza os frames da animação.

## 🧠 Visão Geral

- 🧩 **Plataforma:** Raspberry Pi Pico W (RP2040)
- 🖥️ **Display:** OLED SSD1306 via I2C
- 🎶 **Áudio:** Reprodução de uma melodia via PWM
- 🧵 **Multithreading:** Utilização de ambos os cores do RP2040
- 🧰 **SDK:** [Pico SDK](https://github.com/raspberrypi/pico-sdk)

## 🗂️ Estrutura dos Arquivos

| Arquivo           | Descrição                                                                                                        |
| ----------------- | ---------------------------------------------------------------------------------------------------------------- |
| `display_oled.c`  | Arquivo principal que inicializa os periféricos, configura os núcleos e executa as tarefas da música e animação. |
| `musica.h`        | Define a melodia a ser tocada (notas e durações).                                                                |
| `frames.h`        | Contém os frames do GIF em formato bitmap.                                                                       |
| `ssd1306_i2c.h/c` | Driver para comunicação com o display OLED via I2C.                                                              |
| `ssd1306.h`       | Cabeçalho com definições e comandos auxiliares específicos para o display SSD1306.                               |
| `ssd1306_font.h`  | Fonte utilizada para exibir textos no display (se necessário).                                                   |

## ⚙️ Requisitos

- Raspberry Pi Pico W
- Display OLED 128x64 com chip SSD1306 (I2C)
- Buzzer (ativo ou passivo)
- SDK da Raspberry Pi Pico instalado
- Ambiente de build CMake configurado

## 🚀 Como Compilar

1. Clone o repositório e configure o ambiente do **Pico SDK**:

   ```bash
   git clone https://github.com/Rayver12/BitDogLab-Undertale
   cd BitDogLab-Undertale
   ```

2. Crie uma pasta de build:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Carregue o `.uf2` gerado na memória da Pico.

## 🎵 Núcleo 1: Música

O primeiro core é responsável por:

- Ler a sequência de notas de `musica.h`
- Controlar um pino PWM para gerar sons
- Tocar uma música definida programaticamente

## 🎞️ Núcleo 2: GIF no OLED

O segundo core executa:

- Inicialização do display SSD1306 via I2C
- Exibição sequencial dos frames definidos em `frames.h`
- Atualização contínua do display para criar uma animação fluida

## 💡 Destaques Técnicos

- Uso de `multicore_launch_core1()` para distribuir as tarefas entre os dois núcleos.
- Manipulação de bitmap para exibir frames animados.
- Geração de tons musicais com controle de frequência e duração.
- Sincronização eficiente sem bloqueio entre os núcleos.

## 🧪 Testado com

- Raspberry Pi Pico W
- Display OLED 128x64 com SSD1306 (I2C)
- Buzzer passivo ligado ao GPIO com resistor de proteção

## 📄 Licença

Este projeto é open-source. Use, modifique e distribua conforme desejar. 😉

---

Desenvolvido para explorar o poder dos dois núcleos do RP2040.
