#ifndef MyUI_H
#define MyUI_H
#include <lvgl.h>
#include <memory>
// #include <Arduino.h>
#include "AppMem.hpp"
#define AppMem(addr) v::MyUI::App.addr
namespace v
{
    class MyUI
    {
    public:
        static std::shared_ptr<v::MyUI> myUI;
        bool IsChanged = true;
        static AppMem App;
        // static void initance();
        // virtual ~MyUI();
        virtual void init();
        virtual void refresh();
        void menuInit();
    };
}
#endif