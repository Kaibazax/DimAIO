#include "MyUI.h"
#define ON_TIME_1 0
#define OFF_TIME_1 1
#define OFF_HARD 2
#define ON_TIME_2 3
#define OFF_TIME_2 4
MyUI::MyUI()
{
    return;
};
static void event_handler(lv_event_t *e)
{
}
static const char *btnm_map[] = {"1", "2", "3", "4", "5", "\n",
                                 "6", "7", "8", "9", "0", "\n",
                                 LV_SYMBOL_OK, LV_SYMBOL_CLOSE, LV_SYMBOL_BACKSPACE, ""};
lv_obj_t *btnm1;
void MyUI::event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // uint8_t btn_No = (uint8_t)lv_event_get_user_data(e);
    switch (code)
    {
    case LV_EVENT_PRESSED:
        // lv_label_set_text(label, "The last button event:\nLV_EVENT_PRESSED");
        break;
    case LV_EVENT_CLICKED:
        break;
    case LV_EVENT_LONG_PRESSED:
        // lv_obj_remove_flag(btnm1, LV_OBJ_FLAG_HIDDEN);
        // lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
        break;
    case LV_EVENT_LONG_PRESSED_REPEAT:
        // lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
        break;
    default:
        break;
    }
};
lv_obj_t *chart;
lv_chart_series_t *P1Vac;
lv_chart_series_t *P2Vac;
lv_chart_series_t *InVac;
lv_obj_t *lbl_Fr;
lv_obj_t *lbl_onT1;
lv_obj_t *lbl_onT2;
lv_obj_t *lbl_offHard;
lv_obj_t *lbl_offT1;
lv_obj_t *lbl_offT2;

lv_obj_t *slider_label;
static void removePad(lv_obj_t *obj)
{
    lv_obj_set_style_pad_left(obj, 0, 0);
    lv_obj_set_style_pad_right(obj, 0, 0);
    lv_obj_set_style_pad_top(obj, 0, 0);
    lv_obj_set_style_pad_bottom(obj, 0, 0);
}
void MyUI::ScreenInit()
{
    lv_obj_clean(lv_screen_active());
    static int32_t col_dsc[] = {60, 50, 40, 40, 50, 40, 40, LV_GRID_TEMPLATE_LAST}; // 320-90
    static int32_t row_dsc[] = {20, 100, 40, 40, 40, 150, LV_GRID_TEMPLATE_LAST};   // 240

    /*Create a container with grid*/
    lv_obj_t *cont = lv_obj_create(lv_screen_active());
    lv_obj_remove_style_all(cont);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 320, 240);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    /*Create a transparent wrapper for the chart and the scale.
     *Set a large width, to make it scrollable on the main container*/
    lv_obj_t *wrapper = lv_obj_create(cont);
    lv_obj_remove_style_all(wrapper);
    lv_obj_set_grid_cell(wrapper, LV_GRID_ALIGN_STRETCH, 0, 7, LV_GRID_ALIGN_STRETCH, 1, 1); // Grid layout
    lv_obj_set_flex_flow(wrapper, LV_FLEX_FLOW_COLUMN);

    chart = lv_chart_create(wrapper);
    // removePad(chart);
    lv_obj_set_style_pad_top(chart, 0, 0);
    lv_obj_set_style_pad_bottom(chart, 0, 0);
    lv_obj_set_width(chart, lv_pct(100));
    lv_obj_set_flex_grow(chart, 1);
    // lv_obj_center(chart);
    lv_obj_align_to(chart, wrapper, LV_ALIGN_TOP_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_SCATTER);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -220, 220);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 100);
    lv_obj_set_style_width(chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_height(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 100);
    lv_chart_set_div_line_count(chart, 3, 11);
    P1Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    P2Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    InVac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
    /*Create a scale also with 100% width*/
    lv_obj_t *scale_bottom = lv_scale_create(wrapper);
    lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_size(scale_bottom, lv_pct(100), 20);
    lv_scale_set_total_tick_count(scale_bottom, 11);
    lv_scale_set_major_tick_every(scale_bottom, 2);
    lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);

    static const char *month[] = {"0", "20", "40", "60", "80", "ms"};
    lv_scale_set_text_src(scale_bottom, month);

#pragma region Lable btn
    lbl_Fr = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl_Fr, LV_GRID_ALIGN_STRETCH, 0, 2, LV_GRID_ALIGN_STRETCH, 0, 1); // Grid layout

    lv_obj_t *lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "Xung 1:");
    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "Delay:");
    lv_obj_t *btn = lv_button_create(cont);
    removePad(btn);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lbl_offT1 = lv_label_create(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &offTime1);
    lv_obj_center(lbl_offT1);

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "On:");
    btn = lv_button_create(cont);
    removePad(btn);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 5, 2, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lbl_onT1 = lv_label_create(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &onTime1);
    lv_obj_center(lbl_onT1);

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 3, 1); // Grid layout
    lv_label_set_text(lbl, "Khoản cách 2 xung:");
    btn = lv_button_create(cont);
    removePad(btn);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 3, 2, LV_GRID_ALIGN_STRETCH, 3, 1); // Grid layout
    lbl_offHard = lv_label_create(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &offHard);
    lv_obj_center(lbl_offHard);

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "Xung 2:");
    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "Delay:");
    btn = lv_button_create(cont);
    removePad(btn);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lbl_offT2 = lv_label_create(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &offTime2);
    lv_obj_center(lbl_offT2);

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "On:");
    btn = lv_button_create(cont);
    removePad(btn);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, 5, 2, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lbl_onT2 = lv_label_create(btn);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &onTime2);
    lv_obj_center(lbl_onT2);
#pragma endregion

    btnm1 = lv_buttonmatrix_create(cont);
    lv_obj_set_grid_cell(btnm1, LV_GRID_ALIGN_STRETCH, 0, 7, LV_GRID_ALIGN_STRETCH, 5, 1); // Grid layout
    lv_buttonmatrix_set_map(btnm1, btnm_map);
    lv_buttonmatrix_set_button_width(btnm1, 10, 2); /*Make "Action1" twice as wide as "Action2"*/
    lv_buttonmatrix_set_button_ctrl_all(btnm1, LV_BUTTONMATRIX_CTRL_POPOVER);
    lv_buttonmatrix_set_button_ctrl(btnm1, 10, LV_BUTTONMATRIX_CTRL_CHECKED);
    lv_obj_center(btnm1);
    // lv_obj_add_flag(btnm1, LV_OBJ_FLAG_HIDDEN);

    // /*Create a slider in the center of the display*/
    // lv_obj_t * slider = lv_slider_create(cont);
    // // lv_obj_center(slider);
    // lv_obj_set_grid_cell(slider, LV_GRID_ALIGN_STRETCH, 0, 7, LV_GRID_ALIGN_STRETCH, 5, 1); // Grid layout
    // // lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // lv_obj_set_style_anim_duration(slider, 2000, 0);
    // /*Create a label below the slider*/
    // slider_label = lv_label_create(cont);
    // lv_label_set_text(slider_label, "10");

    // lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
};
void MyUI::ScreenHandle()
{
    if (!IsChanged)
        return;
    lv_chart_remove_series(chart, P1Vac);
    lv_chart_remove_series(chart, P2Vac);
    lv_chart_remove_series(chart, InVac);
    P1Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    P2Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    InVac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_next_value2(chart, P1Vac, offTime1, 0);
    lv_chart_set_next_value2(chart, P2Vac, offTime2, 0);
    for (int16_t i = 0; i < 100; i++)
    {
        double d = 220 * sin(2.0 * PI * i * AcFrequency / 1000);
        if ((i * 1 > offTime1) && (i * 1 < onTime1))
            lv_chart_set_next_value2(chart, P1Vac, i, d);
        else if ((i * 1 > offTime2) && (i * 1 < onTime2))
            lv_chart_set_next_value2(chart, P2Vac, i, d);
        lv_chart_set_next_value2(chart, InVac, i, d);
        /*Directly set points on 'ser2'*/
        // ser2->y_points[i] = lv_rand(50, 90);
    }
    lv_chart_set_next_value2(chart, P1Vac, onTime1, 0);
    lv_chart_set_next_value2(chart, P2Vac, onTime2, 0);
    char str[10];
    sprintf(str, "%2.1fHz", AcFrequency);
    lv_label_set_text(lbl_Fr, str);

    sprintf(str, "%d", onTime1);
    lv_label_set_text(lbl_onT1, str);
    sprintf(str, "%d", offTime1);
    lv_label_set_text(lbl_offT1, str);

    sprintf(str, "%d", offHard);
    lv_label_set_text(lbl_offHard, str);

    sprintf(str, "%d", onTime2);
    lv_label_set_text(lbl_onT2, str);
    sprintf(str, "%d", offTime2);
    lv_label_set_text(lbl_offT2, str);
    IsChanged = false;
};