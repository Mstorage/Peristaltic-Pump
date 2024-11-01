#include "menu.h"
#include "OLED_SPI.h"
#include "Key.h"
#include "UI.h"

#define MENU_X 0       // 菜单位置X
#define MENU_Y 0       // 菜单位置Y
#define MENU_WIDTH 128 // 菜单宽度总像素
#define MENU_HEIGHT 64 // 菜单高度总像素

#define MENU_LINE_H 16 // 行高
#define MENU_PADDING 2 // 内边距

#define MENU_FONT_W 8  // 字体宽度
#define MENU_FONT_H 16 // 字体高度

#define CURSOR_CEILING ((MENU_HEIGHT / MENU_LINE_H) - 1) // 光标限位

/**
  * @brief  命令回调函数，汇总不同命令的具体执行
  * @param  _menu_command:
  * @retval retval：1表示按键按下或者向下（右），-1表示向上（左）
 */
int menu_command_callback(enum _menu_command command, ...)//...为show_X,show_y,show_string
{
  int retval = 0;
  switch (command)
  {
    case BUFFER_DISPLAY:
      OLED_GRAMLODING();
      break;

    case BUFFER_CLEAR:
      OLED_GRAMCLEAR();
      break;

    case SHOW_STRING:
      va_list args;
      va_start(args, command);
      int show_x = va_arg(args, int);
      int show_y = va_arg(args, int);
      char *show_string = va_arg(args, char*);
      OLED_GRAM_Printf(show_x, show_y, FONT_SIZE_16X8, show_string);
      break;
    
    case SHOW_CURSOR:
    {
        /* 提取参数列表 */

        va_list args;
        va_start(args, command);
        int cursor_x = va_arg(args, int);
        int cursor_y = va_arg(args, int);
        int cursor_width = va_arg(args, int);
        int cursor_height = va_arg(args, int);

        /* 按需使用参数 */
        OLED_GRAM_ReversArea(cursor_x, cursor_y, cursor_width, cursor_height);
        break;
    }

        /* Input */
    case GET_EVENT_ENTER:
        retval = Menu_Get_EnterEvent();
        break;

    case GET_EVENT_BACK:
        retval = Menu_Get_BackEvent();
        break;

    case GET_EVENT_WHEEL:
        retval = Menu_Get_RollEvent();
        break;

    default:
      break;
  }
  return retval;
}

/**
  * @brief  运行菜单主函数，判断并显示当前菜单内容。
  * @param  OptionList：当前菜单列表;
  * @retval 1:光标需要向上移动
  * @retval 0:光标不需要移动
 */
void MENU_RunMenu(MENU_OptionTypeDef *OptionList)
{
    int8_t Catch_i = 1;      // 选中下标默认为1,(因为OptionList[0]为"<<<")，对于当前菜单所有条目的第i条
    int8_t Cursor_i = 0;     // 光标下标默认为0, 屏幕中显示的第i条
    int8_t Show_i = 0;       // 显示(遍历)起始下标
    int8_t Wheel_Event = 0;  // 记录菜单滚动事件
    int8_t Option_Max_i = 0; // 选项列表长度
    for (Option_Max_i = 0; OptionList[Option_Max_i].String[0] != '.'; Option_Max_i++) // 计算选项列表长度
    {
        ;
    }
    //Option_Max_i --;

    while(1)
    {

      if (menu_command_callback(GET_EVENT_ENTER))
        {
            /* 如果功能不为空则执行功能,否则返回 */
            if (OptionList[Catch_i].func != NULL)
                OptionList[Catch_i].func(); // 执行选中选项的功能函数
            else
                return;
        }

        if (menu_command_callback(GET_EVENT_BACK))
            return;

        Wheel_Event = menu_command_callback(GET_EVENT_WHEEL);
      
       /* 根据按键事件更改选中下标和光标下标 */
        if (Wheel_Event)
        {
            /* 更新下标 */
            Cursor_i += Wheel_Event;
            Catch_i += Wheel_Event;

            /* 限制选中下标 */
            if (Catch_i > Option_Max_i)
                Catch_i = Option_Max_i;

            if (Catch_i < 0)
                Catch_i = 0;

            /* 限制光标下标 */
            if (Cursor_i > CURSOR_CEILING)
                Cursor_i = CURSOR_CEILING;

            if (Cursor_i > Option_Max_i)
                Cursor_i = Option_Max_i;

            if (Cursor_i > Catch_i)
                Cursor_i = Catch_i;

            if (Cursor_i < 0)
                Cursor_i = 0;
        }
        Show_i = Catch_i - Cursor_i;

        menu_command_callback(BUFFER_CLEAR);

      for(uint8_t i = 0; i <= CURSOR_CEILING; i++) //用于显示当前屏幕范围内所有字符的主循环
      {
        if (Show_i + i > Option_Max_i)
          break;

        uint8_t show_x = MENU_X + MENU_PADDING; 
        uint8_t show_y = MENU_Y + i * MENU_FONT_H;
        char *show_str = OptionList[Show_i + i].String;
        menu_command_callback(SHOW_STRING, show_x, show_y, show_str);
      }
      /* 显示光标 */
        uint8_t cursor_x = MENU_X;
        uint8_t cursor_y = MENU_Y + (Cursor_i * MENU_LINE_H);
        uint8_t cursor_width = MENU_WIDTH;
        uint8_t cursor_height = MENU_LINE_H;
        menu_command_callback(SHOW_CURSOR, cursor_x, cursor_y, cursor_width, cursor_height);

        menu_command_callback(BUFFER_DISPLAY); // 更新缓存至显示器
    }

}
/**
  * @brief  用于检测菜单光标滚动的函数
  * @param  none;
  * @retval 1:光标需要向上移动
  * @retval -1:光标需要向下移动
  * @retval 0:光标不动
 */
int8_t Menu_Get_RollEvent(void)
{
	if (Key_Left_Get()) 
	{
		return 1;
	}
	if (Key_Right_Get()) 
	{
		return -1;
	}
	return 0; 
}
/**
  * @brief  用于检测菜单确认的函数
  * @param  none;
  * @retval 1:菜单确认
  * @retval 0:none
 */
int8_t Menu_Get_EnterEvent(void) // 菜单确认
{
	return Key_Enter_Get(); 
}
/**
  * @brief  用于检测菜单返回的函数
  * @param  none;
  * @retval 1:菜单返回
  * @retval 0:none
 */
int8_t Menu_Get_BackEvent(void) // 菜单返回
{
	return Key_Back_Get(); 
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
