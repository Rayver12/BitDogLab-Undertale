#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "musica.h"
#include "frames.h"

// Pinos do display OLED
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

const uint LED_PIN = 13;    // Pino do LED
const uint BUZZER_PIN = 21; // Pino do buzzer

// Função para configuração do Buzzer
void configurar_buzzer(int pin)
{
    gpio_set_function(pin, GPIO_FUNC_PWM); // Escolhe o pino para PWM (buzzer)
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config configuracao = pwm_get_default_config();
    pwm_config_set_clkdiv(&configuracao, 0.5); // Reduz o divisor de clock para aumentar a frequência
    pwm_init(slice_num, &configuracao, true);
    pwm_set_gpio_level(pin, 0); // Inicializa o nível de PWM em 0 (sem som)
}

// Função para ajustar a frequência usada
void ajustar_frequencia(int pin, int freq)
{
    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_div = clock_get_hz(clk_sys) / (freq * 5000);
    pwm_set_wrap(slice_num, 5000 - 1);    // Ajusta o wrap para o valor adequado para a frequência
    pwm_set_clkdiv(slice_num, clock_div); // Configura o divisor de clock com base na frequência
}

// Função responsável por emitir o som no pino escolhido
void emitir_som(int pin, int freq, int duracao_ms)
{
    ajustar_frequencia(pin, freq); // Ajusta a frequência
    pwm_set_gpio_level(pin, 45);   // Ativa o PWM com um nível de duty cycle (volume)
    sleep_ms(duracao_ms);          // Toca a nota pelo tempo determinado
    pwm_set_gpio_level(pin, 0);    // Desliga o PWM após a duração da nota
}

// Código que vai rodar no primeiro núcleo do microcontrolador
void core0_main()
{
    // Inicializa o LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Inicializa o Buzzer
    configurar_buzzer(BUZZER_PIN);

    // Loop para tocar a música
    // Notas, tempo e pausa definidos em musica.h
    while (true)
    {
        for (int i = 0; i < sizeof(notas) / sizeof(notas[0]); i++)
        {
            // Toca a nota correspondente no buzzer
            emitir_som(BUZZER_PIN, notas[i], tempo[i]);
            // printf("Nota %d: %d Hz\n", i + 1, notas[i]);
            //  Acende o LED enquanto o buzzer está tocando
            gpio_put(LED_PIN, 1);
            sleep_ms(tempo[i] / 15); // Mantém o LED aceso durante a duração da nota
            gpio_put(LED_PIN, 0);
            sleep_ms(pausa[i] / 15); // Pausa entre as notas, com o LED apagado
        }
    }
}
// função que vai rodar no segundo núcleo do microcontrolador
void core1_main()
{
    // Inicialize a tela OLED
    // Inicialização do i2c
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Processo de inicialização completo do OLED SSD1306
    ssd1306_init();

    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    struct render_area frame_area = {
        start_column : 0,
        end_column : ssd1306_width - 1,
        start_page : 0,
        end_page : ssd1306_n_pages - 1
    };

    calculate_render_area_buffer_length(&frame_area);

    // zera o display inteiro
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);

restart:

    ssd1306_t ssd_bm;
    ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
    ssd1306_config(&ssd_bm);

    // Loop para imprimir o gif no display (bitmaps no arquivo frames.h)

    while (true)
    {
        ssd1306_draw_bitmap(&ssd_bm, frame_00);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_01);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_02);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_03);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_04);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_05);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_06);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_07);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_08);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_09);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_10);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_11);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_12);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_13);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_14);
        sleep_ms(70);
        ssd1306_draw_bitmap(&ssd_bm, frame_15);
        sleep_ms(70);
    }

    while (true)
    {
        sleep_ms(1000);
    };
}

int main()
{
    stdio_init_all(); // Inicializa comunicação serial (opcional)
    sleep_ms(1000);
    printf("Undertale - Megalovania");

    // Executa a exibição do GIF no núcleo 1
    multicore_launch_core1(core1_main);

    // Lança o núcleo 0 para tocar música e piscar o LED
    core0_main();

    return 0; // Nunca será alcançado
}