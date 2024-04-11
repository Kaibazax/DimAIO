#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
SPIClass touchscreenSpi = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;
LV_FONT_DECLARE(lv_font_vn);
/*Set to your screen resolution*/
#define TFT_HOR_RES 320
#define TFT_VER_RES 240

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    /*Call it to tell LVGL you are ready*/
    lv_disp_flush_ready(disp);
}
/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    if (touchscreen.touched())
    {
        TS_Point p = touchscreen.getPoint();
        // Some very basic auto calibration so it doesn't go out of range
        if (p.x < touchScreenMinimumX)
            touchScreenMinimumX = p.x;
        if (p.x > touchScreenMaximumX)
            touchScreenMaximumX = p.x;
        if (p.y < touchScreenMinimumY)
            touchScreenMinimumY = p.y;
        if (p.y > touchScreenMaximumY)
            touchScreenMaximumY = p.y;
        // Map this to the pixel position
        data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, TFT_HOR_RES); /* Touchscreen X calibration */
        data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, TFT_VER_RES); /* Touchscreen Y calibration */
        data->state = LV_INDEV_STATE_PRESSED;
        /*
        Serial.print("Touch x ");
        Serial.print(data->point.x);
        Serial.print(" y ");
        Serial.println(data->point.y);
        */
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static void event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);

    switch (code)
    {
    case LV_EVENT_PRESSED:
        lv_label_set_text(label, "The last button event:\nLV_EVENT_PRESSED");
        break;
    case LV_EVENT_CLICKED:
        lv_label_set_text(label, "The last button event:\nLV_EVENT_CLICKED");
        break;
    case LV_EVENT_LONG_PRESSED:
        lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
        break;
    case LV_EVENT_LONG_PRESSED_REPEAT:
        lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
        break;
    default:
        break;
    }
}

/**
 * Handle multiple events
 */
void lv_example_event_2(void)
{
    lv_obj_t *btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(btn, 100, 50);
    lv_obj_center(btn);

    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, LV_SYMBOL_OK "\xE1\xBA\xA4n v\xC3\xA0o");
    lv_obj_center(btn_label);

    lv_obj_t *info_label = lv_label_create(lv_screen_active());
    lv_label_set_text(info_label, "Nút cuối được ấn là:\nNone");

    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, info_label);
}
lv_indev_t *indev;     // Touchscreen input device
uint8_t *draw_buf;     // draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0; // Used to track the tick timer

void setup()
{
    // Some basic info on the Serial console
    String LVGL_Arduino = "LVGL demo ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.begin(115200);
    Serial.println(LVGL_Arduino);

    // Initialise the touchscreen
    touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
    touchscreen.setRotation(3);                                                /* Inverted landscape orientation to match screen */

    // Initialise LVGL
    lv_init();
    draw_buf = new uint8_t[DRAW_BUF_SIZE];
    lv_display_t *disp;
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
    // Initialize the XPT2046 input device driver
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
    lv_theme_t *th = lv_theme_default_init(disp ,                                       /*Use the DPI, size, etc from this display*/
                                           lv_color_hex(0x6039a8), lv_color_hex(0x238636), /*Primary and secondary palette*/
                                           true,                                         /*Light or dark mode*/
                                           &lv_font_vn);                       /*Small, normal, large fonts*/

    lv_display_set_theme(disp, th);

    lv_example_event_2();
    // Done
    Serial.println("Setup done");
}

void loop()
{
    lv_tick_inc(millis() - lastTick); // Update the tick timer. Tick is new for LVGL 9
    lastTick = millis();
    lv_timer_handler(); // Update the UI
    delay(5);
}