#include "Home.hpp"
using namespace v;
Home::~Home()
{
    Serial.printf("Decontruc Home");
};
void Home::init()
{
    lv_obj_clean(lv_screen_active());
    MyUI::menuInit();
}
void Home::refresh()
{
}