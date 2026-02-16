#include "pico/stdlib.h"
#include "lcd/lcd.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

// Perform initialisation
int pico_led_init(void)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

// Turn the led on or off
void pico_set_led(bool led_on)
{
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// int main()
// {
//     int rc = pico_led_init();
//     hard_assert(rc == PICO_OK);
//     while (true)
//     {
//         pico_set_led(true);
//         sleep_ms(LED_DELAY_MS);
//         pico_set_led(false);
//         sleep_ms(LED_DELAY_MS);
//     }
// }

#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430

#define LIGHT_GRAY 0xb596
#define DARK_GRAY 0x5acb

#define GAMEBOY_WIDTH 160
#define GAMEBOY_HEIGHT 144

int lcd_test(void)
{
    lcd_system_init();

    lcd_scan_dir orientation = D2U_L2R;
    lcd_init(orientation, 800);

    lcd_clear(BLACK);

    int x_offset = (SCREEN_HEIGHT - GAMEBOY_WIDTH) / 2; // screen height because of rotation
    int x_end = SCREEN_HEIGHT - x_offset - 1;

    int y_offset = (SCREEN_WIDTH - GAMEBOY_HEIGHT) / 2; // screen width because of rotation
    int y_end = SCREEN_WIDTH - y_offset - 1;

    lcd_set_window(x_offset, y_offset, x_end, y_end);

    COLOR colors[4] = {WHITE, LIGHT_GRAY, DARK_GRAY, BLACK};

    int framebuffer_len = GAMEBOY_HEIGHT * GAMEBOY_WIDTH;
    uint8_t buf[framebuffer_len * 2];
    for (uint32_t i = 0; i < framebuffer_len; i++)
    {
        int line = i / GAMEBOY_WIDTH;
        COLOR color = colors[line % 4];
        buf[i * 2] = color >> 8;
        buf[i * 2 + 1] = color & 0xFF;
    }

    lcd_write_buffer(buf, framebuffer_len * 2);

    while (1)
    {
        // lcd_write_color(RED, GAMEBOY_WIDTH * GAMEBOY_HEIGHT);
        // sleep_ms(2000);
        // lcd_write_color(BLUE, GAMEBOY_WIDTH * GAMEBOY_HEIGHT);
        // sleep_ms(2000);
    }

    return 0;
}

int main(void)
{
    while (1)
    {
        lcd_test();
    }
    return 0;
}