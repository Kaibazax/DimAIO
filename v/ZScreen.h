#ifndef ZScreen_H
#define ZScreen_H
#include <lvgl.h>
#include "AppMem.h"
#define AppMem(addr) v::ZScreen::App.addr
namespace v
{
    class ZScreen
    {
    public:
        bool IsChanged = true;
        static AppMem App;
        // static void initance();
        // virtual ~ZScreen();
        virtual void init();
        virtual void refresh();
    };
}
#endif