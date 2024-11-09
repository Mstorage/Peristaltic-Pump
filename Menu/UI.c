#include "UI.h"
#include "OLED_SPI.h"
#include "Key.h"
#include "Flash_W25Q.h"

void UI_Main(void)
{
    ;
}
void UI_MotorRevers(void)
{
        OLED_GRAM_UIAnimation(BMP_MotorDirR_Adr, BMP_MotorDirR_Len);
}
void UI_ManualMode(void)
{
    ;
}
void UI_RunTimeSwitch(void)
{
    ;
}
void UI_CurrentMonitor(void)
{
    ;
}
void UI_Information(void)
{
    ;
}
void UI_Animation_BadApple(void)
{
    OLED_GRAM_Animation(BMP_BadApple_Adr, BMP_BadApple_Len);
}