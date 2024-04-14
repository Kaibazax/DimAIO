#ifndef APPMEM_H
#define APPMEM_H
namespace v
{
    struct FlashMem
    {
        int32_t delayP1 = 15; // 100us
        int32_t onP1 = 30;
        int32_t space2P = 45;
        int32_t offTime2 = 45;
        int32_t onTime2 = 60;
        int32_t dDlT1 = 42;
        int32_t dOnT1 = 83; // 167
        int32_t dOffH = 600;
        int32_t dDlT2 = 0;
        int32_t dOnT2 = 167;
    };
    struct AppMem : FlashMem
    {
        float AcFrequency = 60.0f;
        float Ac_ms = 16.7f;
    };
}
#endif