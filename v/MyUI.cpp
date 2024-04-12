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
static const char *btnm_map[] = {LV_SYMBOL_LEFT, LV_SYMBOL_PLUS, LV_SYMBOL_MINUS, LV_SYMBOL_RIGHT, ""};
lv_obj_t *btnm1;
lv_obj_t *chart;
lv_chart_series_t *FocusRange;
lv_chart_series_t *P1Vac;
lv_chart_series_t *P2Vac;
lv_chart_series_t *InVac;
lv_obj_t *lbl_Fr;
lv_obj_t *lbl_onT1;
lv_obj_t *lbl_onT2;
lv_obj_t *lbl_offHard;
lv_obj_t *lbl_offT1;
lv_obj_t *lbl_offT2;

lv_group_t *g;
MyUI *_this;
int8_t selectedRange = 0;
void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *sb;
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t id = lv_buttonmatrix_get_selected_button(obj);
        sb = lv_group_get_focused(g);
        int32_t step = lv_spinbox_get_step(sb);
        Serial.printf("Step:%d", step);
        switch (id)
        {
        case 0:
            if (step > 100)
            {
                lv_group_focus_prev(g);
                sb = lv_group_get_focused(g);
                lv_spinbox_set_step(sb, 1);
            }
            else
            {
                lv_spinbox_step_prev(sb);
            }
            break;

        case 1:
            lv_spinbox_increment(sb);
            break;

        case 2:
            lv_spinbox_decrement(sb);
            break;

        case 3:
            if (step < 10)
            {
                lv_group_focus_next(g);
                sb = lv_group_get_focused(g);
                lv_spinbox_set_step(sb, 1000);
            }
            else
            {
                lv_spinbox_step_next(sb);
            }
            break;
        }
    }
}
void setAllPad(lv_obj_t *obj, int32_t i)
{
    lv_obj_set_style_pad_left(obj, 0, 0);
    lv_obj_set_style_pad_right(obj, 0, 0);
    lv_obj_set_style_pad_top(obj, 0, 0);
    lv_obj_set_style_pad_bottom(obj, 0, 0);
}
void event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    int32_t *var = (int32_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_VALUE_CHANGED)
    {
        *var = lv_spinbox_get_value(obj);
        _this->dInt2mInt();
        _this->IsChanged = true;
        Serial.printf("changed %d %d\n", _this->IsChanged, _this->offTime1);
    }
    if (code == LV_EVENT_FOCUSED)
    {
        selectedRange = lv_obj_get_index(obj);
        Serial.printf("index %d \n", selectedRange);
    }
};
void default_spinbox(lv_obj_t *spinbox, int32_t *value)
{
    lv_spinbox_set_range(spinbox, 0, 10000);
    lv_spinbox_set_digit_format(spinbox, 4, 3);
    lv_spinbox_step_prev(spinbox);
    lv_spinbox_set_value(spinbox, *value);
    // lv_spinbox_set_digit_step_direction(spinbox, LV_DIR_RIGHT);
    setAllPad(spinbox, 0);
    lv_obj_remove_state(spinbox, LV_STATE_FOCUSED);

    lv_obj_set_style_bg_color(spinbox, lv_color_hex(0x282b30), LV_PART_CURSOR);
    lv_obj_set_style_bg_color(spinbox, lv_palette_main(LV_PALETTE_PURPLE), LV_STATE_FOCUSED | LV_PART_CURSOR);
    lv_obj_add_event_cb(spinbox, event_cb, LV_EVENT_ALL, value);
    // lv_obj_get_style_bg_color
};
void MyUI::ScreenInit()
{
    _this = this;
    dInt2mInt();
    lv_obj_clean(lv_screen_active());
    static int32_t col_dsc[] = {60, 50, 40, 40, 50, 40, 40, LV_GRID_TEMPLATE_LAST}; // 320-90
    static int32_t row_dsc[] = {20, 100, 20, 20, 20, 60, LV_GRID_TEMPLATE_LAST};    // 240
#pragma region Layout
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
#pragma endregion
#pragma region chart
    chart = lv_chart_create(wrapper);
    // setAllPad(chart,0);
    lv_obj_set_style_pad_top(chart, 0, 0);
    lv_obj_set_style_pad_bottom(chart, 0, 0);
    lv_obj_set_width(chart, lv_pct(200));
    lv_obj_set_flex_grow(chart, 1);
    // lv_obj_center(chart);
    lv_obj_align_to(chart, wrapper, LV_ALIGN_TOP_MID, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_SCATTER);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -220, 220);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0, 200);
    lv_obj_set_style_width(chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_height(chart, 0, LV_PART_INDICATOR);
    lv_chart_set_point_count(chart, 200);
    lv_chart_set_div_line_count(chart, 3, 21);
    FocusRange = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    P1Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    P2Vac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    InVac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
    /*Create a scale also with 100% width*/
    lv_obj_t *scale_bottom = lv_scale_create(wrapper);
    lv_scale_set_mode(scale_bottom, LV_SCALE_MODE_HORIZONTAL_BOTTOM);
    lv_obj_set_size(scale_bottom, lv_pct(200), 20);
    lv_scale_set_total_tick_count(scale_bottom, 11);
    lv_scale_set_major_tick_every(scale_bottom, 1);
    lv_obj_set_style_pad_hor(scale_bottom, lv_chart_get_first_point_center_offset(chart), 0);

    static const char *month[] = {"0", "20", "40", "60", "80", "100", "120", "140", "160", "180", "ms"};
    lv_scale_set_text_src(scale_bottom, month);
#pragma endregion
#pragma region Lable btn
    g = lv_group_create();
    lbl_Fr = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl_Fr, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 0, 1); // Grid layout

    lv_obj_t *lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "Xung 1:");
    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "Delay:");
    lv_obj_t *spinbox = lv_spinbox_create(cont);
    default_spinbox(spinbox, &dDlT1);
    lv_group_add_obj(g, spinbox);
    lv_obj_set_grid_cell(spinbox, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    // lbl_offT1 = lv_label_create(spinbox);
    // lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, &offTime1);
    // lv_obj_center(lbl_offT1);

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout
    lv_label_set_text(lbl, "On:");
    spinbox = lv_spinbox_create(cont);
    default_spinbox(spinbox, &dOnT1);
    lv_group_add_obj(g, spinbox);
    lv_obj_set_grid_cell(spinbox, LV_GRID_ALIGN_STRETCH, 5, 2, LV_GRID_ALIGN_STRETCH, 2, 1); // Grid layout

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 3, LV_GRID_ALIGN_STRETCH, 3, 1); // Grid layout
    lv_label_set_text(lbl, "Khoản cách 2 xung:");
    spinbox = lv_spinbox_create(cont);
    default_spinbox(spinbox, &dOffH);
    lv_group_add_obj(g, spinbox);
    lv_obj_set_grid_cell(spinbox, LV_GRID_ALIGN_STRETCH, 3, 2, LV_GRID_ALIGN_STRETCH, 3, 1); // Grid layout

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "Xung 2:");
    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "Delay:");
    spinbox = lv_spinbox_create(cont);
    default_spinbox(spinbox, &dDlT2);
    lv_group_add_obj(g, spinbox);
    lv_obj_set_grid_cell(spinbox, LV_GRID_ALIGN_STRETCH, 2, 2, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout

    lbl = lv_label_create(cont);
    lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
    lv_label_set_text(lbl, "On:");
    spinbox = lv_spinbox_create(cont);
    default_spinbox(spinbox, &dOnT2);
    lv_group_add_obj(g, spinbox);
    lv_obj_set_grid_cell(spinbox, LV_GRID_ALIGN_STRETCH, 5, 2, LV_GRID_ALIGN_STRETCH, 4, 1); // Grid layout
#pragma endregion

    btnm1 = lv_buttonmatrix_create(cont);
    lv_obj_set_grid_cell(btnm1, LV_GRID_ALIGN_STRETCH, 0, 7, LV_GRID_ALIGN_STRETCH, 5, 1); // Grid layout
    lv_buttonmatrix_set_map(btnm1, btnm_map);
    lv_buttonmatrix_set_button_ctrl_all(btnm1, LV_BUTTONMATRIX_CTRL_POPOVER);
    lv_obj_set_style_border_side(btnm1, LV_BORDER_SIDE_NONE, LV_PART_MAIN);
    setAllPad(btnm1, 2);
    lv_color_t c = lv_obj_get_style_bg_color(spinbox, LV_PART_MAIN);
    lv_obj_add_event_cb(btnm1, event_handler, LV_EVENT_ALL, NULL);
};
void MyUI::ScreenHandle()
{
    if (!IsChanged)
        return;
    lv_chart_remove_series(chart, FocusRange);
    lv_chart_remove_series(chart, P1Vac);
    lv_chart_remove_series(chart, P2Vac);
    lv_chart_remove_series(chart, InVac);
    FocusRange = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_PRIMARY_Y);
    P1Vac = lv_chart_add_series(chart, lv_palette_main(selectedRange == 6 ? LV_PALETTE_YELLOW : LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    P2Vac = lv_chart_add_series(chart, lv_palette_main(selectedRange == 13 ? LV_PALETTE_YELLOW : LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
    InVac = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_BLUE_GREY), LV_CHART_AXIS_PRIMARY_Y);
    switch (selectedRange)
    {
    case 4:
        lv_chart_set_next_value2(chart, FocusRange, 0, 0);
        lv_chart_set_next_value2(chart, FocusRange, offTime1 / 10, 0);
        break;
    case 8:
        lv_chart_set_next_value2(chart, FocusRange, onTime1 / 10, 0);
        lv_chart_set_next_value2(chart, FocusRange, offHard / 10, 0);
        break;
    case 11:
        lv_chart_set_next_value2(chart, FocusRange, offHard / 10, 0);
        lv_chart_set_next_value2(chart, FocusRange, offTime2 / 10, 0);
        break;
    }
    lv_chart_set_next_value2(chart, P1Vac, offTime1 / 10, 0);
    lv_chart_set_next_value2(chart, P2Vac, offTime2 / 10, 0);
    for (int16_t i = 0; i < 200; i++)
    {
        double d = 220 * sin(2.0 * PI * i * AcFrequency / 1000);
        if ((i * 1 > offTime1 / 10) && (i * 1 < onTime1 / 10))
            lv_chart_set_next_value2(chart, P1Vac, i, d);
        else if ((i * 1 > offTime2 / 10) && (i * 1 < onTime2 / 10))
            lv_chart_set_next_value2(chart, P2Vac, i, d);
        lv_chart_set_next_value2(chart, InVac, i, d);
        /*Directly set points on 'ser2'*/
        // ser2->y_points[i] = lv_rand(50, 90);
    }
    lv_chart_set_next_value2(chart, P1Vac, onTime1 / 10, 0);
    lv_chart_set_next_value2(chart, P2Vac, onTime2 / 10, 0);
    char str[20];
    sprintf(str, "%2.1fHz T=%2.1fms", AcFrequency, 1000 / AcFrequency);
    lv_label_set_text(lbl_Fr, str);

    // sprintf(str, "%d", offTime1);
    // lv_label_set_text(lbl_offT1, str);
    // sprintf(str, "%d", onTime1);
    // lv_label_set_text(lbl_onT1, str);

    // sprintf(str, "%d", offHard);
    // lv_label_set_text(lbl_offHard, str);

    // sprintf(str, "%d", onTime2);
    // lv_label_set_text(lbl_onT2, str);
    // sprintf(str, "%d", offTime2);
    // lv_label_set_text(lbl_offT2, str);
    IsChanged = false;
};
void MyUI::mInt2dInt()
{
    dDlT1 = offTime1;
    dOnT1 = onTime1 - offTime1;
    dOffH = offHard - onTime1;
    dDlT2 = offTime2 - offHard;
    dOnT2 = onTime2 - offTime2;
};
void MyUI::dInt2mInt()
{
    int32_t sum = dDlT1;
    offTime1 = sum;
    sum += dOnT1;
    onTime1 = sum;
    sum += dOffH;
    double d = sum * AcFrequency / 10000;
    int32_t i = (int32_t)d + 1;
    sum = i / AcFrequency * 10000;
    offHard = sum;
    sum += dDlT2;
    offTime2 = sum;
    sum += dOnT2;
    onTime2 = sum;
}