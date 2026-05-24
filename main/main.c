#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/spi_master.h"
#include "driver/gpio.h"

#include "esp_err.h"
#include "esp_log.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
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

#define LCD_H_RES 320
#define LCD_V_RES 172

static const char *TAG = "LCD";

static uint16_t frame_buffer[LCD_H_RES * 40];

static void fill_color(uint16_t color)
{
    for (int i = 0; i < LCD_H_RES * 40; i++) {
        frame_buffer[i] = color;
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "LCD START");

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
    gpio_set_level(PIN_NUM_RST, 1);

    // =========================
    // SPI BUS
    // =========================
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_CLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz =
            LCD_H_RES * 40 * sizeof(uint16_t),
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
        .pclk_hz = 40000000,
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
    // PANEL CONFIG
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
    // OFFSET LCD WAVESHARE
    // =========================
    ESP_ERROR_CHECK(
        esp_lcd_panel_set_gap(
            panel_handle,
            0,
            34
        )
    );

    // =========================
    // LANDSCAPE MODE
    // =========================
    ESP_ERROR_CHECK(
        esp_lcd_panel_swap_xy(
            panel_handle,
            true
        )
    );

    ESP_ERROR_CHECK(
        esp_lcd_panel_mirror(
            panel_handle,
            false,
            true
        )
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
    // TEST DISPLAY
    // =========================
    while (1) {

        // MERAH
        fill_color(0xF800);

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            0,
            320,
            40,
            frame_buffer
        );

        // HIJAU
        fill_color(0x07E0);

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            40,
            320,
            80,
            frame_buffer
        );

        // BIRU
        fill_color(0x001F);

        esp_lcd_panel_draw_bitmap(
            panel_handle,
            0,
            80,
            320,
            120,
            frame_buffer
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}