static esp_err_t panel_init(
    esp_lcd_panel_t *panel
)
{
    st7789_panel_t *st7789 =
        __containerof(panel, st7789_panel_t, base);

    esp_lcd_panel_io_handle_t io =
        st7789->io;

    // SOFTWARE RESET
    esp_lcd_panel_io_tx_param(io, 0x01, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(150));

    // SLEEP OUT
    esp_lcd_panel_io_tx_param(io, 0x11, NULL, 0);
    vTaskDelay(pdMS_TO_TICKS(120));

    // MADCTL
    uint8_t madctl = 0x00;
    esp_lcd_panel_io_tx_param(io, 0x36, &madctl, 1);

    // RGB565
    uint8_t colmod = 0x55;
    esp_lcd_panel_io_tx_param(io, 0x3A, &colmod, 1);

    // Porch
    uint8_t porch[] = {0x0c, 0x0c, 0x00, 0x33, 0x33};
    esp_lcd_panel_io_tx_param(io, 0xB2, porch, 5);

    // Gate control
    uint8_t gate = 0x75;
    esp_lcd_panel_io_tx_param(io, 0xB7, &gate, 1);

    // VCOM
    uint8_t vcom = 0x19;
    esp_lcd_panel_io_tx_param(io, 0xBB, &vcom, 1);

    // LCM
    uint8_t lcm = 0x2C;
    esp_lcd_panel_io_tx_param(io, 0xC0, &lcm, 1);

    // VDV + VRH
    uint8_t vrh_en[] = {0x01};
    esp_lcd_panel_io_tx_param(io, 0xC2, vrh_en, 1);

    // VRH
    uint8_t vrh = 0x12;
    esp_lcd_panel_io_tx_param(io, 0xC3, &vrh, 1);

    // VDV
    uint8_t vdv = 0x20;
    esp_lcd_panel_io_tx_param(io, 0xC4, &vdv, 1);

    // FR CTRL
    uint8_t fr = 0x0F;
    esp_lcd_panel_io_tx_param(io, 0xC6, &fr, 1);

    // Power
    uint8_t power[] = {0xA4, 0xA1};
    esp_lcd_panel_io_tx_param(io, 0xD0, power, 2);

    // Positive gamma
    uint8_t gamma1[] = {
        0xD0,0x04,0x0D,0x11,
        0x13,0x2B,0x3F,0x54,
        0x4C,0x18,0x0D,0x0B,
        0x1F,0x23
    };

    esp_lcd_panel_io_tx_param(io, 0xE0, gamma1, 14);

    // Negative gamma
    uint8_t gamma2[] = {
        0xD0,0x04,0x0C,0x11,
        0x13,0x2C,0x3F,0x44,
        0x51,0x2F,0x1F,0x1F,
        0x20,0x23
    };

    esp_lcd_panel_io_tx_param(io, 0xE1, gamma2, 14);

    // INVERSION ON
    esp_lcd_panel_io_tx_param(io, 0x21, NULL, 0);

    // DISPLAY ON
    esp_lcd_panel_io_tx_param(io, 0x29, NULL, 0);

    vTaskDelay(pdMS_TO_TICKS(100));

    return ESP_OK;
}