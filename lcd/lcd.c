#include "lcd.h"

LCDProperties properties;

static void lcd_reset()
{
	gpio_put(LCD_RST_PIN, 1);
	sleep_ms(500);
	gpio_put(LCD_RST_PIN, 0);
	sleep_ms(500);
	gpio_put(LCD_RST_PIN, 1);
	sleep_ms(500);
}

static void lcd_init_registers()
{
	lcd_write_register(0x11);
	sleep_ms(100);
	lcd_write_register(0x36);
	lcd_write_data(0x00);
	lcd_write_register(0x3a);
	lcd_write_data(0x55);
	lcd_write_register(0xb2);
	lcd_write_data(0x0c);
	lcd_write_data(0x0c);
	lcd_write_data(0x00);
	lcd_write_data(0x33);
	lcd_write_data(0x33);
	lcd_write_register(0xb7);
	lcd_write_data(0x35);
	lcd_write_register(0xbb);
	lcd_write_data(0x28);
	lcd_write_register(0xc0);
	lcd_write_data(0x3c);
	lcd_write_register(0xc2);
	lcd_write_data(0x01);
	lcd_write_register(0xc3);
	lcd_write_data(0x0b);
	lcd_write_register(0xc4);
	lcd_write_data(0x20);
	lcd_write_register(0xc6);
	lcd_write_data(0x0f);
	lcd_write_register(0xD0);
	lcd_write_data(0xa4);
	lcd_write_data(0xa1);
	lcd_write_register(0xe0);
	lcd_write_data(0xd0);
	lcd_write_data(0x01);
	lcd_write_data(0x08);
	lcd_write_data(0x0f);
	lcd_write_data(0x11);
	lcd_write_data(0x2a);
	lcd_write_data(0x36);
	lcd_write_data(0x55);
	lcd_write_data(0x44);
	lcd_write_data(0x3a);
	lcd_write_data(0x0b);
	lcd_write_data(0x06);
	lcd_write_data(0x11);
	lcd_write_data(0x20);
	lcd_write_register(0xe1);
	lcd_write_data(0xd0);
	lcd_write_data(0x02);
	lcd_write_data(0x07);
	lcd_write_data(0x0a);
	lcd_write_data(0x0b);
	lcd_write_data(0x18);
	lcd_write_data(0x34);
	lcd_write_data(0x43);
	lcd_write_data(0x4a);
	lcd_write_data(0x2b);
	lcd_write_data(0x1b);
	lcd_write_data(0x1c);
	lcd_write_data(0x22);
	lcd_write_data(0x1f);
	lcd_write_register(0x55);
	lcd_write_data(0xB0);
	lcd_write_register(0x29);
}

static void lcd_set_gram_scan_way(lcd_scan_dir orientation)
{
	uint16_t MemoryAccessReg_Data = 0; // addr:0x36

	switch (orientation)
	{
	case L2R_U2D:
		/* Memory access control: MY = 0, MX = 0, MV = 0, ML = 0 RGB = 0 MH = 0 NN = 0 NN = 0*/
		MemoryAccessReg_Data = 0x00;
		break;
	case D2U_L2R:
		/* Memory access control: MY = 1, MX = 0, MV = 1, ML = 0 RGB = 0 MH = 0 NN = 0 NN = 0*/
		MemoryAccessReg_Data = 0xA0;
		break;
	case R2L_D2U:
		/* Memory access control: MY = 1, MX = 1, MV = 0, ML = 0 RGB = 0 MH = 0 NN = 0 NN = 0*/
		MemoryAccessReg_Data = 0xc0;
		break;
	case U2D_R2L:
		/* Memory access control: MY = 0, MX = 1, MV = 1, ML = 0 RGB = 0 MH = 0 NN = 0 NN = 0*/
		MemoryAccessReg_Data = 0x60;
		break;
	}
	properties.orientation = orientation;

	// Get GRAM and LCD width and height
	// 240*320,vertical default
	if (orientation == L2R_U2D || orientation == R2L_D2U)
	{
		properties.columns = SCREEN_WIDTH;
		properties.rows = SCREEN_HEIGHT;
	}
	else
	{
		properties.columns = SCREEN_HEIGHT;
		properties.rows = SCREEN_WIDTH;
	}

	lcd_write_register(0x36);
	lcd_write_data(MemoryAccessReg_Data);
}

/**
 * Setup pico gpio pins, configure pico SPI interface
 */
uint8_t lcd_system_init(void)
{
	stdio_init_all();
	gpio_init(LCD_RST_PIN);
	gpio_set_dir(LCD_RST_PIN, GPIO_OUT);
	gpio_init(LCD_DC_PIN);
	gpio_set_dir(LCD_DC_PIN, GPIO_OUT);
	gpio_init(LCD_BKL_PIN);
	gpio_set_dir(LCD_BKL_PIN, GPIO_OUT);
	gpio_init(LCD_CS_PIN);
	gpio_set_dir(LCD_CS_PIN, GPIO_OUT);
	gpio_init(TP_CS_PIN);
	gpio_set_dir(TP_CS_PIN, GPIO_OUT);
	gpio_init(TP_IRQ_PIN);
	gpio_set_dir(TP_IRQ_PIN, GPIO_IN);
	gpio_init(SD_CS_PIN);
	gpio_set_dir(SD_CS_PIN, GPIO_OUT);
	gpio_set_pulls(TP_IRQ_PIN, true, false);

	gpio_put(TP_CS_PIN, 1);
	gpio_put(LCD_CS_PIN, 1);
	gpio_put(LCD_BKL_PIN, 1);
	gpio_put(SD_CS_PIN, 1);
	// spi_init(SPI_PORT, 4000000);
	spi_init(SPI_PORT, 60000000);
	spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
	gpio_set_function(LCD_CLK_PIN, GPIO_FUNC_SPI);
	gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);
	gpio_set_function(LCD_MISO_PIN, GPIO_FUNC_SPI);

	return 0;
}

/**
 * SPI setup, display reset, register sequence
 * @param orientation orientation of the display
 * @param bl_val brightness of the backlight
 */
void lcd_init(lcd_scan_dir orientation, uint16_t brightness)
{
	lcd_reset();

	lcd_init_registers();

	if (brightness > 1000)
		brightness = 1000;
	// LCD_SetBackLight(LCD_BLval);

	lcd_set_gram_scan_way(orientation); // Set the display scan and color transfer modes
	sleep_ms(200);
}

/**
 * Fill screen with solid color
 * @note Will reset window to full screen
 * @param color fill color
 */
void lcd_clear(COLOR color)
{
	lcd_set_window(0, 0, properties.columns, properties.rows);
	lcd_write_color(color, SCREEN_WIDTH * SCREEN_HEIGHT);
}

/**
 * Define a region to draw to, performs CASET + RASET
 * @param x_start x start position
 * @param y_start y start position
 * @param x_end x end position
 * @param y_end x end position
 */
void lcd_set_window(POINT x_start, POINT y_start, POINT x_end, POINT y_end)
{
	// Set X positions
	lcd_write_register(0x2A);
	lcd_write_data(x_start >> 8);
	lcd_write_data(x_start & 0xFF);
	lcd_write_data((x_end - 1) >> 8);
	lcd_write_data((x_end - 1) & 0xFF);

	// Set Y positions
	lcd_write_register(0x2B);
	lcd_write_data(y_start >> 8);
	lcd_write_data(y_start & 0xFF);
	lcd_write_data((y_end - 1) >> 8);
	lcd_write_data((y_end - 1) & 0xFF);

	lcd_write_register(0x2C);
}

/**
 * Stream raw pixel data to current window
 */
void lcd_write_buffer(uint8_t *data, uint32_t length)
{
	gpio_put(LCD_DC_PIN, 1);
	gpio_put(LCD_CS_PIN, 0);

	spi_write_blocking(spi1, data, length);

	gpio_put(LCD_CS_PIN, 1);
}

/**
 * Fill current window with one colour (e.g clear/borders)
 * @param COLOR fill color
 */
void lcd_write_color(COLOR color, uint32_t length)
{
	gpio_put(LCD_DC_PIN, 1);
	gpio_put(LCD_CS_PIN, 0);

	uint8_t buf[length * 2];
	for (uint32_t i = 0; i < length; i++)
	{
		buf[i * 2] = color >> 8;
		buf[i * 2 + 1] = color & 0xFF;
	}

	spi_write_blocking(spi1, buf, length * 2);

	gpio_put(LCD_CS_PIN, 1);
}

/**
 * Prepare register to be written to
 * @param reg register address
 */
void lcd_write_register(uint8_t reg)
{
	gpio_put(LCD_DC_PIN, 0);
	gpio_put(LCD_CS_PIN, 0);
	spi_write_blocking(spi1, &reg, 1);
	gpio_put(LCD_CS_PIN, 1);
}

/**
 * Write data to previously prepared register
 * @param data data to write
 */
void lcd_write_data(uint8_t data)
{
	gpio_put(LCD_DC_PIN, 1);
	gpio_put(LCD_CS_PIN, 0);
	spi_write_blocking(spi1, &data, 1);
	gpio_put(LCD_CS_PIN, 1);
}

/**
 * Write string to display
 * @param x_start x starting position
 * @param y_start y starting position
 * @param label string to display
 * @param font font to use
 * @param background color behind label
 * @param foreground text color
 */
void lcd_display_string(POINT x_start, POINT y_start, const char *label, sFONT *font, COLOR background, COLOR foreground)
{
	if (x_start > properties.columns || y_start > properties.rows)
	{
		return;
	}

	POINT x = x_start;
	POINT y = y_start;

	while (*label != '\0')
	{
		if ((x + font->Width) > properties.columns)
		{
			x = x_start;
			y += font->Height;
		}

		if ((y + font->Height) > properties.rows)
		{
			x = x_start;
			y = y_start;
		}

		lcd_display_character(x, y, *label, font, background, foreground);

		label++;
		x += font->Width;
	}
}

void lcd_display_character(POINT Xpoint, POINT Ypoint, const char Acsii_Char,
						   sFONT *Font, COLOR Color_Background, COLOR Color_Foreground)
{
	POINT Page, Column;

	if (Xpoint > properties.columns || Ypoint > properties.rows)
	{
		return;
	}

	uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
	const unsigned char *ptr = &Font->table[Char_Offset];

	for (Page = 0; Page < Font->Height; Page++)
	{
		for (Column = 0; Column < Font->Width; Column++)
		{

			// To determine whether the font background color and screen background color is consistent
			if (FONT_BACKGROUND == Color_Background)
			{ // this process is to speed up the scan
				if (*ptr & (0x80 >> (Column % 8)))
					lcd_draw_point(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
			}
			else
			{
				if (*ptr & (0x80 >> (Column % 8)))
				{
					lcd_draw_point(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
				}
				else
				{
					lcd_draw_point(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
				}
			}
			// One pixel is 8 bits
			if (Column % 8 == 7)
				ptr++;
		} /* Write a line */
		if (Font->Width % 8 != 0)
			ptr++;
	} /* Write all */
}

void lcd_draw_point(POINT Xpoint, POINT Ypoint, COLOR Color, DOT_PIXEL Dot_Pixel, DOT_STYLE DOT_STYLE)
{
	if (Xpoint > properties.columns || Ypoint > properties.rows)
	{
		return;
	}

	uint16_t XDir_Num, YDir_Num;
	if (DOT_STYLE == DOT_STYLE_DFT)
	{
		for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++)
		{
			for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++)
			{
				lcd_draw_pixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
			}
		}
	}
	else
	{
		for (XDir_Num = 0; XDir_Num < Dot_Pixel; XDir_Num++)
		{
			for (YDir_Num = 0; YDir_Num < Dot_Pixel; YDir_Num++)
			{
				lcd_draw_pixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
			}
		}
	}
}

void lcd_draw_pixel(POINT x, POINT y, COLOR color)
{
	if ((x <= properties.columns) && (y <= properties.rows))
	{
		lcd_set_window(x, y, x, y);
		lcd_set_color(color, 1, 1);
	}
}

void lcd_set_color(COLOR color, POINT x, POINT y)
{
	int buffer_len = x * y;
	uint8_t buf[buffer_len * 2];
	for (uint32_t i = 0; i < buffer_len; i++)
	{
		buf[i * 2] = color >> 8;
		buf[i * 2 + 1] = color & 0xFF;
	}
	lcd_write_buffer(buf, (uint32_t)x * (uint32_t)y * 2);
}