#include <stdlib.h>
#include <sys/cdefs.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "esp_check.h"
#include "esp_log.h"

#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_commands.h"

#include "Vernon_ST7789T.h"

static const char *TAG = "ST7789T";

typedef struct {
    esp_lcd_panel_t base;
    esp_lcd_panel_io_handle_t io;

    int reset_gpio_num;
    int x_gap;
    int y_gap;

    uint8_t madctl_val;
    uint8_t colmod_val;

} st7789t_panel_t;

static esp_err_t panel_st7789t_del(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789t_reset(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789t_init(esp_lcd_panel_t *panel);
static esp_err_t panel_st7789t_draw_bitmap(
    esp_lcd_panel_t *panel,
    int x_start,
    int y_start,
    int x_end,
    int y_end,
    const void *color_data
);

static esp_err_t panel_st7789t_mirror(
    esp_lcd_panel_t *panel,
    bool mirror_x,
    bool mirror_y
);

static esp_err_t panel_st7789t_swap_xy(
    esp_lcd_panel_t *panel,
    bool swap_axes
);

static esp_err_t panel_st7789t_set_gap(
    esp_lcd_panel_t *panel,
    int x_gap,
    int y_gap
);

static esp_err_t panel_st7789t_disp_on_off(
    esp_lcd_panel_t *panel,
    bool on_off
);

esp_err_t esp_lcd_new_panel_st7789t(
    const esp_lcd_panel_io_handle_t io,
    const esp_lcd_panel_dev_st7789t_config_t *panel_dev_config,
    esp_lcd_panel_handle_t *ret_panel
)
{
    st7789t_panel_t *st7789t = calloc(1, sizeof(st7789t_panel_t));

    st7789t->io = io;
    st7789t->reset_gpio_num = panel_dev_config->reset_gpio_num;

    st7789t->madctl_val = 0x00;
    st7789t->colmod_val = 0x55;

    st7789t->base.del = panel_st7789t_del;
    st7789t->base.reset = panel_st7789t_reset;
    st7789t->base.init = panel_st7789t_init;
    st7789t->base.draw_bitmap = panel_st7789t_draw_bitmap;
    st7789t->base.mirror = panel_st7789t_mirror;
    st7789t->base.swap_xy = panel_st7789t_swap_xy;
    st7789t->base.set_gap = panel_st7789t_set_gap;
    st7789t->base.disp_on_off = panel_st7789t_disp_on_off;

    *ret_panel = &(st7789t->base);

    return ESP_OK;
}

static esp_err_t panel_st7789t_del(esp_lcd_panel_t *panel)
{
    free(panel);
    return ESP_OK;
}

static esp_err_t panel_st7789t_reset(esp_lcd_panel_t *panel)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    gpio_set_direction(st7789t->reset_gpio_num, GPIO_MODE_OUTPUT);

    gpio_set_level(st7789t->reset_gpio_num, 0);
    vTaskDelay(pdMS_TO_TICKS(20));

    gpio_set_level(st7789t->reset_gpio_num, 1);
    vTaskDelay(pdMS_TO_TICKS(20));

    return ESP_OK;
}

static esp_err_t panel_st7789t_init(esp_lcd_panel_t *panel)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    esp_lcd_panel_io_handle_t io = st7789t->io;

    // Sleep out
    esp_lcd_panel_io_tx_param(io, 0x11, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Landscape
    esp_lcd_panel_io_tx_param(io, 0x36, (uint8_t []){0x70}, 1);

    // RGB565
    esp_lcd_panel_io_tx_param(io, 0x3A, (uint8_t []){0x55}, 1);

    // Inversion ON
    esp_lcd_panel_io_tx_param(io, 0x21, NULL, 0);

    // Display ON
    esp_lcd_panel_io_tx_param(io, 0x29, NULL, 0);

    vTaskDelay(pdMS_TO_TICKS(20));

    ESP_LOGI(TAG, "ST7789 initialized");

    return ESP_OK;
}

static esp_err_t panel_st7789t_draw_bitmap(
    esp_lcd_panel_t *panel,
    int x_start,
    int y_start,
    int x_end,
    int y_end,
    const void *color_data
)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    esp_lcd_panel_io_handle_t io = st7789t->io;

    x_start += st7789t->x_gap;
    x_end += st7789t->x_gap;

    y_start += st7789t->y_gap;
    y_end += st7789t->y_gap;

    esp_lcd_panel_io_tx_param(io, 0x2A, (uint8_t[]) {
        (x_start >> 8) & 0xFF,
        x_start & 0xFF,
        ((x_end - 1) >> 8) & 0xFF,
        (x_end - 1) & 0xFF,
    }, 4);

    esp_lcd_panel_io_tx_param(io, 0x2B, (uint8_t[]) {
        (y_start >> 8) & 0xFF,
        y_start & 0xFF,
        ((y_end - 1) >> 8) & 0xFF,
        (y_end - 1) & 0xFF,
    }, 4);

    size_t len = (x_end - x_start) *
                 (y_end - y_start) * 2;

    esp_lcd_panel_io_tx_color(io, 0x2C, color_data, len);

    return ESP_OK;
}

static esp_err_t panel_st7789t_mirror(
    esp_lcd_panel_t *panel,
    bool mirror_x,
    bool mirror_y
)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    if (mirror_x)
        st7789t->madctl_val |= 0x40;
    else
        st7789t->madctl_val &= ~0x40;

    if (mirror_y)
        st7789t->madctl_val |= 0x80;
    else
        st7789t->madctl_val &= ~0x80;

    esp_lcd_panel_io_tx_param(
        st7789t->io,
        0x36,
        (uint8_t[]) { st7789t->madctl_val },
        1
    );

    return ESP_OK;
}

static esp_err_t panel_st7789t_swap_xy(
    esp_lcd_panel_t *panel,
    bool swap_axes
)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    if (swap_axes)
        st7789t->madctl_val |= 0x20;
    else
        st7789t->madctl_val &= ~0x20;

    esp_lcd_panel_io_tx_param(
        st7789t->io,
        0x36,
        (uint8_t[]) { st7789t->madctl_val },
        1
    );

    return ESP_OK;
}

static esp_err_t panel_st7789t_set_gap(
    esp_lcd_panel_t *panel,
    int x_gap,
    int y_gap
)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    st7789t->x_gap = x_gap;
    st7789t->y_gap = y_gap;

    return ESP_OK;
}

static esp_err_t panel_st7789t_disp_on_off(
    esp_lcd_panel_t *panel,
    bool on_off
)
{
    st7789t_panel_t *st7789t =
        __containerof(panel, st7789t_panel_t, base);

    esp_lcd_panel_io_tx_param(
        st7789t->io,
        on_off ? 0x29 : 0x28,
        NULL,
        0
    );

    return ESP_OK;
}