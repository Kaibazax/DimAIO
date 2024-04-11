#include "MyUI.h"
MyUI::MyUI()
{
    return;
};
void MyUI::event_cb(lv_event_t *e)
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
};
lv_obj_t *chart;
lv_chart_series_t *OutVdc;
lv_chart_series_t *InVdc;
lv_obj_t *lbl;
void MyUI::ScreenInit()
{
    // lv_obj_clean(lv_screen_active());
    // lv_obj_t *tabview;
    // tabview = lv_tabview_create(lv_screen_active());
    // lv_tabview_set_tab_bar_size(tabview, 50);
    // /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    // lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Chạy");
    // lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Xung");
    // lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Motor");
    // lv_obj_t *cont_col = lv_obj_create(tab2);
    // lv_obj_set_size(cont_col, 320, 600);
    // lv_obj_center(cont_col);
    // lv_obj_set_flex_flow(cont_col, LV_FLEX_FLOW_COLUMN);
    chart = lv_chart_create(lv_screen_active());
    lv_obj_set_size(chart, 320, 75);
    lv_obj_center(chart);
    // lv_obj_align_to(chart, cont_col, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -220, 220);
    // lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 100);
    lv_obj_set_style_width(chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_height(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 100);
    OutVdc = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    InVdc = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
    for (int16_t i = 0; i < 100; i++)
    {
        double d = 220 * sin(2.0 * PI * i / AcFrequency);
        lv_chart_set_next_value(chart, OutVdc, d);
        lv_chart_set_next_value(chart, InVdc, d);
        Serial.printf("%f ", 220 * sin(2.0 * PI * i * 4 / AcFrequency));
        /*Directly set points on 'ser2'*/
        // ser2->y_points[i] = lv_rand(50, 90);
    }
    lbl = lv_label_create(lv_screen_active());
    char Fr[20] = "";
    sprintf(Fr, "%2.1fHz %d %d %d %d", AcFrequency, offTime1, onTime1, offTime2, onTime2);
    lv_label_set_text(lbl, Fr);
    // IsChanged = false;
    // lv_obj_t *cont_row = lv_obj_create(tab2);
};
void MyUI::ScreenHandle()
{
    if (!IsChanged)
        return;
    for (int16_t i = 0; i < 100; i++)
    {
        double d = 220 * sin(2.0 * PI * i * AcFrequency / 1000);
        bool on = (i * 1 > offTime1) && (i * 1 < onTime1);
        on = on || ((i * 1 > offTime2) && (i * 1 < onTime2));
        lv_chart_set_next_value(chart, OutVdc, on ? d : 0);
        lv_chart_set_next_value(chart, InVdc, d);
        Serial.printf("%f ", on ? d : 0);
        /*Directly set points on 'ser2'*/
        // ser2->y_points[i] = lv_rand(50, 90);
    }
    char Fr[30];
    sprintf(Fr, "%2.1fHz %dms %dms %dms %dms", AcFrequency, offTime1, onTime1, offTime2, onTime2);
    lv_label_set_text(lbl, Fr);
    IsChanged = false;
};