#define VERBOSE 1
#pragma region Tft init
#include "v_MyUI.hpp"
#include "v_AcChart.hpp"
#include "v_Home.hpp"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <memory>
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

lv_indev_t *indev;     // Touchscreen input device
uint8_t *draw_buf;     // draw_buf is allocated on heap otherwise the static area is too big on ESP32 at compile
uint32_t lastTick = 0; // Used to track the tick timer
#pragma endregion
// #include "v/Home.cpp"
// #include "v/MyUI.h"
uint32_t last_usTick = 0;
bool lastI = true;
uint32_t last_render = 0;
void taskHW(void *parameter)
{
    Serial.printf("task HW core:%d\n", xPortGetCoreID());
    delay(1000);
    last_usTick = micros();
    delayMicroseconds(50);
    for (;;)
    {
        uint32_t usTick = micros();
        if (last_usTick < usTick && !v::MyUI::myUI->IsChanged)
        {
            float acms = AppMem(Ac_ms);
            float acf = AppMem(AcFrequency);
            acms *= 0.9f;
            acms += (usTick - last_usTick) / 10000;
            // acms /= 10;
            if (acms > 1)
            {
                acf = 1000.0f / acms;
                AppMem(Ac_ms) = acms;
                // Serial.printf("AcF Update:%2.2fms %2.2fHz %dus\n", acms, acf, usTick);

                if (acf > 1 && acf < 200 && acms < 1000)
                {
                    //
                    AppMem(AcFrequency) = acf;
                    if (usTick - last_render > 500000)
                    {
                        v::MyUI::myUI->IsChanged = true;
                        last_render = usTick;
                    }
                }
            }
        }
        if (digitalRead(0) != lastI || last_usTick > usTick)
        {
            last_usTick = usTick;
            lastI = digitalRead(0);
        }
        delayMicroseconds(47);
    }
}
TaskHandle_t TaskHWHandle;
lv_display_t *disp;
void setup()
{
    // Some basic info on the Serial console
    pinMode(0, INPUT_PULLDOWN);
    String LVGL_Arduino = "LVGL";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
#if VERBOSE
    Serial.begin(115200);
    Serial.println(LVGL_Arduino);
#endif
    // Initialise the touchscreen
    touchscreenSpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
    touchscreen.begin(touchscreenSpi);                                         /* Touchscreen init */
    touchscreen.setRotation(3);                                                /* Inverted landscape orientation to match screen */

    // Initialise LVGL
    lv_init();
    draw_buf = new uint8_t[DRAW_BUF_SIZE];
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
    // Initialize the XPT2046 input device driver
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touchpad_read);
    lv_theme_t *th = lv_theme_default_init(disp,                                           /*Use the DPI, size, etc from this display*/
                                           lv_color_hex(0x6039a8), lv_color_hex(0x238636), /*Primary and secondary palette*/
                                           true,                                           /*Light or dark mode*/
                                           &lv_font_vn);                                   /*Small, normal, large fonts*/

    lv_display_set_theme(disp, th);

// Done
#if VERBOSE
    Serial.println("Setup done");
#endif
    v::MyUI::myUI = std::make_shared<v::AcChart>();
    v::MyUI::myUI->init();
    xTaskCreatePinnedToCore(
        taskHW,        /* Function to implement the task */
        "Task1",       /* Name of the task */
        10000,         /* Stack size in words */
        NULL,          /* Task input parameter */
        0,             /* Priority of the task */
        &TaskHWHandle, /* Task handle. */
        0);            /* Core where the task should run */
}

void loop()
{
    if (Serial.available())
    {
        byte buff[8];
        Serial.readBytes(buff, 8);
        AppMem(AcFrequency) = (float)buff[0];
        AppMem(onP1) = buff[2];
        AppMem(delayP1) = buff[1];
        AppMem(onTime2) = buff[4];
        AppMem(offTime2) = buff[3];
        v::MyUI::myUI->IsChanged = true;
        // lv_obj_invalidate(lv_screen_active());
        Serial.printf("Main loop core:%d\n", xPortGetCoreID());
    }
    lv_disp_enable_invalidation(disp, !v::MyUI::myUI->IsChanged);
    if (v::MyUI::myUI->IsChanged)
    {
        // lv_disp_enable_invalidation(disp, false);
        v::MyUI::myUI->refresh();
        // lv_disp_enable_invalidation(disp, true);
    }
    else
        lv_timer_handler();           // Update the UI
    lv_tick_inc(millis() - lastTick); // Update the tick timer. Tick is new for LVGL 9
    lastTick = millis();
    delay(5);
}