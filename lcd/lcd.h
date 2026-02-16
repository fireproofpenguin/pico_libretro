#ifndef __LCD_H
#define __LCD_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

// Dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Colors
#define COLOR uint16_t

#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000

// Sizes
#define POINT uint16_t
#define LENGTH uint16_t

// Orientation
typedef enum
{
    L2R_U2D = 0, // 0°
    D2U_L2R,     // 90°
    R2L_D2U,     // 180°
    U2D_R2L,     // 270°
} lcd_scan_dir;

typedef struct
{
    LENGTH columns;
    LENGTH rows;
    lcd_scan_dir orientation;
    POINT LCD_X_Adjust; // LCD x actual display position calibration
    POINT LCD_Y_Adjust; // LCD y actual display position calibration
} LCDProperties;

// Pins
#define LCD_RST_PIN 15
#define LCD_DC_PIN 8
#define LCD_CS_PIN 9
#define LCD_CLK_PIN 10
#define LCD_MOSI_PIN 11
#define LCD_MISO_PIN 12
#define LCD_BKL_PIN 13
#define TP_CS_PIN 16
#define TP_IRQ_PIN 17
#define SD_CS_PIN 22
#define SPI_PORT spi1

// Functions
uint8_t lcd_system_init(void);
void lcd_init(lcd_scan_dir orientation, uint16_t brightness);
void lcd_clear(COLOR color);
void lcd_set_window(POINT x_start, POINT y_start, POINT x_end, POINT y_end);
void lcd_write_color(COLOR color, uint32_t length);
void lcd_write_register(uint8_t reg);
void lcd_write_data(uint8_t data);
#endif