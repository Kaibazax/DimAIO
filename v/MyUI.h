#ifndef MyUI_H
#define MyUI_H
#include <lvgl.h>
// namespace v
// {
class MyUI
{

public:
    float AcFrequency = 55.5f;
    int32_t offTime1 = 15;
    int32_t onTime1 = 30;
    int32_t offHard = 45;
    int32_t offTime2 = 45;
    int32_t onTime2 = 60;
    bool IsChanged = true;
    MyUI();
    void ScreenInit();
    void ScreenHandle();
    static void event_cb(lv_event_t *e);
};
// }
#endif