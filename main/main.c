#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "Vernon_ST7789T.h"

#define PIN_NUM_MOSI  6
#define PIN_NUM_CLK   7
#define PIN_NUM_CS    14
#define PIN_NUM_DC    15
#define PIN_NUM_RST   21
#define PIN_NUM_BL    22

#define LCD_H_RES     320
#define LCD_V_RES     172

void app_main(void)
{
    gpio_set_direction(PIN_NUM_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_BL, 1);

    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_CLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 80 * sizeof(uint16_t),
    };

    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);

    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = 40000000,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };

    esp_lcd_new_panel_io_spi(
        (esp_lcd_spi_bus_handle_t)SPI2_HOST,
        &io_config,
        &io_handle
    );

    esp_lcd_panel_handle_t panel_handle = NULL;

    esp_lcd_panel_dev_st7789t_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };

    esp_lcd_new_panel_st7789t(
        io_handle,
        &panel_config,
        &panel_handle
    );

    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);

    // LANDSCAPE
    esp_lcd_panel_swap_xy(panel_handle, true);
    esp_lcd_panel_mirror(panel_handle, true, false);
    esp_lcd_panel_set_gap(panel_handle, 35, 0);

    esp_lcd_panel_disp_on_off(panel_handle, true);

    static uint16_t buffer[LCD_H_RES * 20];

    while (1) {

        for (int i = 0; i < LCD_H_RES * 20; i++) {
            buffer[i] = 0xF800;
        }

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            0,
            LCD_H_RES,
            20,
            buffer
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 0; i < LCD_H_RES * 20; i++) {
            buffer[i] = 0x07E0;
        }

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            20,
            LCD_H_RES,
            40,
            buffer
        );

        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 0; i < LCD_H_RES * 20; i++) {
            buffer[i] = 0x001F;
        }

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            40,
            LCD_H_RES,
            60,
            buffer
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}