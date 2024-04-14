#ifndef MyUI_CPP
#define MyUI_CPP
#include "v_MyUI.hpp"
#include "v_AcChart.hpp"
#include "v_Home.hpp"
using namespace v;
AppMem MyUI::App;
std::shared_ptr<v::MyUI> MyUI::myUI;
void MyUI::init(){};
void button_event_cb(lv_event_t *e)
{
    int id = (int)lv_event_get_user_data(e);
    Serial.printf("clicked id %d\n", id);
    switch (id)
    {
    case 0:
        MyUI::myUI = std::make_shared<Home>();
        MyUI::myUI->init();
        break;
    case 4:
        MyUI::myUI = std::make_shared<AcChart>();
        MyUI::myUI->init();
        break;
    }
}
void float_button_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_user_data(e);
    if (lv_obj_has_flag(obj, LV_OBJ_FLAG_HIDDEN))
    {
        lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}
lv_obj_t *createBtn(lv_obj_t *cont, int col, int row, const char *icon, const char *text, int id)
{
    lv_obj_t *btn = lv_button_create(cont);
    // lv_obj_set_user_data(btn, id);

    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1); // Grid layout
    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, icon); // icon
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, -10);
    lbl = lv_label_create(btn);
    lv_label_set_text(lbl, text); // icon
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 10);
    lv_obj_set_width(lbl, 80);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
    lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, (intptr_t *)id);
    return btn;
}
void MyUI::menuInit()
{
    static int32_t col_dsc[] = {80, 80, 80, LV_GRID_TEMPLATE_LAST}; // 320-90
    static int32_t row_dsc[] = {50, 50, LV_GRID_TEMPLATE_LAST};     // 240
    lv_obj_t *Menu_bg = lv_obj_create(lv_screen_active());
    lv_obj_set_size(Menu_bg, 320, 240);
    lv_obj_set_style_bg_color(Menu_bg, lv_color_hex(0xcccccc), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(Menu_bg, LV_OPA_50, LV_PART_MAIN);
    // lv_obj_set_style_opa(Menu_bg, LV_OPA_50, LV_PART_MAIN);

    lv_obj_t *cont = lv_obj_create(Menu_bg);
    // lv_obj_remove_style_all(cont);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 290, 200);
    lv_obj_center(cont);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    createBtn(cont, 0, 0, LV_SYMBOL_HOME, "Trang chủ", 0);
    createBtn(cont, 1, 0, LV_SYMBOL_SETTINGS, "Cài đặt Motor", 1);
    createBtn(cont, 2, 0, LV_SYMBOL_CHARGE, "Cài đặt Hàn", 2);
    createBtn(cont, 0, 1, LV_SYMBOL_DRIVE, "Cài đặt drive", 3);
    createBtn(cont, 1, 1, LV_SYMBOL_WARNING, "Cài đặt Hàn Nâng cao", 4);
    createBtn(cont, 2, 1, LV_SYMBOL_WIFI, "Update Firmware", 5);

    lv_obj_add_flag(Menu_bg, LV_OBJ_FLAG_HIDDEN);

#pragma region Float Btn
    lv_obj_t *float_btn = lv_button_create(lv_screen_active());
    lv_obj_set_size(float_btn, 30, 30);
    lv_obj_add_flag(float_btn, LV_OBJ_FLAG_FLOATING);
    lv_obj_align(float_btn, LV_ALIGN_TOP_RIGHT, -10, 10);
    lv_obj_add_event_cb(float_btn, float_button_event_cb, LV_EVENT_CLICKED, Menu_bg);
    lv_obj_set_style_radius(float_btn, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_opa(float_btn, LV_OPA_70, LV_PART_MAIN);
    // setAllPad(float_btn,)
    lv_obj_t *lbl = lv_label_create(float_btn);
    lv_label_set_text(lbl, LV_SYMBOL_LIST);
    lv_obj_center(lbl);
#pragma endregion
};
void MyUI::refresh(){};
#endif