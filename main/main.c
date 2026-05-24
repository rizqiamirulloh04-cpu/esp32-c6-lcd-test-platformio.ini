#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "esp_err.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

#include "Vernon_ST7789T.h"

#define LCD_HOST SPI2_HOST

// WAVESHARE ESP32-C6 LCD 1.47
#define PIN_NUM_MOSI 6
#define PIN_NUM_CLK  7
#define PIN_NUM_CS   14
#define PIN_NUM_DC   15
#define PIN_NUM_RST  21
#define PIN_NUM_BL   22

void app_main(void)
{
    // =========================
    // BACKLIGHT
    // =========================
    gpio_reset_pin(PIN_NUM_BL);
    gpio_set_direction(PIN_NUM_BL, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_BL, 1);

    // =========================
    // RESET
    // =========================
    gpio_reset_pin(PIN_NUM_RST);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);

    // =========================
    // SPI BUS
    // =========================
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_CLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 320 * 172 * 2,
    };

    ESP_ERROR_CHECK(
        spi_bus_initialize(
            LCD_HOST,
            &buscfg,
            SPI_DMA_CH_AUTO
        )
    );

    // =========================
    // LCD IO
    // =========================
    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = 10000000,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_io_spi(
            (esp_lcd_spi_bus_handle_t)LCD_HOST,
            &io_config,
            &io_handle
        )
    );

    // =========================
    // PANEL
    // =========================
    esp_lcd_panel_handle_t panel_handle = NULL;

    esp_lcd_panel_dev_st7789t_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .bits_per_pixel = 16,
    };

    ESP_ERROR_CHECK(
        esp_lcd_new_panel_st7789t(
            io_handle,
            &panel_config,
            &panel_handle
        )
    );

    // =========================
    // RESET PANEL
    // =========================
    ESP_ERROR_CHECK(
        esp_lcd_panel_reset(panel_handle)
    );

    // =========================
    // INIT PANEL
    // =========================
    ESP_ERROR_CHECK(
        esp_lcd_panel_init(panel_handle)
    );

    // =========================
    // DISPLAY ON
    // =========================
    ESP_ERROR_CHECK(
        esp_lcd_panel_disp_on_off(
            panel_handle,
            true
        )
    );

    // =========================
    // FULL RED TEST
    // =========================
    static uint16_t buf[320 * 172];

    for (int i = 0; i < (320 * 172); i++) {
        buf[i] = 0xF800;
    }

    ESP_ERROR_CHECK(
        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            0,
            320,
            172,
            buf
        )
    );

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}