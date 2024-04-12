#ifndef MyUI_H
#define MyUI_H
#include <lvgl.h>
// namespace v
// {
class MyUI
{

public:
    float AcFrequency = 60.0f;
    int32_t offTime1 = 15;//100us
    int32_t onTime1 = 30;
    int32_t offHard = 45;
    int32_t offTime2 = 45;
    int32_t onTime2 = 60;
    int32_t dDlT1 = 42;
    int32_t dOnT1 = 83;//167
    int32_t dOffH = 600;
    int32_t dDlT2 = 0;
    int32_t dOnT2 = 167;
    bool IsChanged = true;
    MyUI();
    void ScreenInit();
    void ScreenHandle();
    void mInt2dInt();
    void dInt2mInt();
    // void event_cb(lv_event_t *e);
    // void default_spinbox(lv_obj_t *spinbox, int32_t *value);
};
// }
#endif