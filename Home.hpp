#ifndef Home_H
#define Home_H
#include "MyUI.hpp"
namespace v
{
    class Home : public MyUI
    {
    public:
        ~Home();
        virtual void init();
        virtual void refresh();
    };
}
#endif