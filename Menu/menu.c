#include "menu.h"
#include "OLED_SPI.h"

void menu_command_callback(enum _menu_command command, ...)
{
  int reval = 0;
  switch (command)
  {
    case BUFFER_DISPLAY:
      OLED_GRAMLODING();
      break;

    case BUFFER_CLEAR:
      OLED_Clear();
      break;

    case SHOW_STRING:
      va_list args;
      va_start(args, command);
      uint8_t show_x = va_arg(args, int);
      uint8_t show_y = va_arg(args, int);
      uint8_t *show_string = va_arg(args, char*);
      OLED_Printf(show_x, show_y, FONT_SIZE_16X8, show_string);
      break;
    
    default:
      break;
  }
}

/**
  * @brief  运行主菜单
  * @param  none;
  * @retval none;
 */
void MENU_RunMainMenu(void)
{
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"System", MENU_RunSystemSetting},      // 系统设置
                                                   {"Display", MENU_RunDisplaySetting},    // 显示设置
                                                   {"Time switch", UI_RunTimeSwitch},      // 定时系统
                                                   {"Current", UI_CurrentMonitor},         // 电流监视
                                                   {"Information", UI_Information},        // 关于我们
                                                   {".."}};

    MENU_RunMenu(MENU_OptionList);
}

/**
  * @brief  运行系统设置菜单
  * @param  none;
  * @retval none;
 */
void MENU_RunSystemSetting(void)
{
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"MotorRevers", UI_MotorRevers},      // 电机反转
                                                   {"ManualMode", UI_ManualMode},      // 手动模式
                                                   {"Animation", MENU_RunAnimation}, // 开机动画
                                                   {".."}};

    MENU_RunMenu(MENU_OptionList);
}

/**
  * @brief  运行显示设置菜单
  * @param  none;
  * @retval none;
 */
void MENU_RunDisplaySetting(void)
{
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"Revers", NULL},     // 反色显示
                                                   {"Style", NULL},     // 显示风格
                                                   {".."}};

    MENU_RunMenu(MENU_OptionList);
}

/**
  * @brief  运行开机动画设置菜单
  * @param  none;
  * @retval none;
 */
void MENU_RunAnimation(void)
{
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"Default", NULL},     // 默认动画
                                                   {"LOGO", NULL},      // logo动画
                                                   {"BadApple", NULL}, // badapplel
                                                   {".."}};

    MENU_RunMenu(MENU_OptionList);
}
