#ifndef __LCD_H
#define __LCD_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "fonts/fonts.h"

// Dimensions
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Colors
#define COLOR uint16_t

#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000

#define LCD_BACKGROUND COLOR_WHITE  // Default background color
#define FONT_BACKGROUND COLOR_WHITE // Default font background color
#define FONT_FOREGROUND COLOR_BLACK // Default font foreground color

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

typedef enum
{
    DOT_PIXEL_1X1 = 1, // dot pixel 1 x 1
    DOT_PIXEL_2X2,     // dot pixel 2 X 2
    DOT_PIXEL_3X3,     // dot pixel 3 X 3
    DOT_PIXEL_4X4,     // dot pixel 4 X 4
    DOT_PIXEL_5X5,     // dot pixel 5 X 5
    DOT_PIXEL_6X6,     // dot pixel 6 X 6
    DOT_PIXEL_7X7,     // dot pixel 7 X 7
    DOT_PIXEL_8X8,     // dot pixel 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT DOT_PIXEL_1X1 // Default dot pilex

typedef enum
{
    DOT_FILL_AROUND = 1, // dot pixel 1 x 1
    DOT_FILL_RIGHTUP,    // dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT DOT_FILL_AROUND // Default dot pilex

// Functions
uint8_t lcd_system_init(void);
void lcd_init(lcd_scan_dir orientation, uint16_t brightness);
void lcd_clear(COLOR color);
void lcd_set_window(POINT x_start, POINT y_start, POINT x_end, POINT y_end);
void lcd_write_buffer(uint8_t *data, uint32_t length);
void lcd_write_color(COLOR color, uint32_t length);
void lcd_write_register(uint8_t reg);
void lcd_write_data(uint8_t data);
void lcd_display_string(POINT x_start, POINT y_start, const char *label, sFONT *font, COLOR background, COLOR foreground);
void lcd_display_character(POINT Xpoint, POINT Ypoint, const char Acsii_Char,
                           sFONT *Font, COLOR Color_Background, COLOR Color_Foreground);
void lcd_draw_point(POINT Xpoint, POINT Ypoint, COLOR Color, DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE);
void lcd_draw_pixel(POINT x, POINT y, COLOR color);
void lcd_set_color(COLOR color, POINT x, POINT y);
#endif