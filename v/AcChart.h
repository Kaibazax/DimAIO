#ifndef MyUI_H
#define MyUI_H
#include "ZScreen.cpp"
namespace v
{
    class AcChart : public ZScreen
    {
    public:
        ~AcChart();
        virtual void init();
        virtual void refresh();
        void mInt2dInt();
        void dInt2mInt();
        // void event_cb(lv_event_t *e);
        // void default_spinbox(lv_obj_t *spinbox, int32_t *value);
    };
}
#endif