#ifndef _MENU_H_
#define _MENU_H_

#ifndef NULL
#define NULL ((void *)0)
#endif

#include "UI.h"
#include <stdint.h>

typedef struct _MENU_OptionTypeDef // 选项结构体
{
    char *String;       // 选项字符串
    void (*func)(void); // 选项功能 函数指针
} MENU_OptionTypeDef;

/**********************************************************/
enum _menu_command
{
    BUFFER_DISPLAY, // 无参无返, 显示显存中的内容
    BUFFER_CLEAR,   // 无参无返, 清空屏幕
    SHOW_STRING,    // 可变参数列表对应顺序: x, y, string
    SHOW_CURSOR,    // 可变参数列表对应顺序: x, y, width, height;

    GET_EVENT_ENTER,     // 返回布尔
    GET_EVENT_BACK,      // 返回布尔
    GET_EVENT_DIRECTION, // 返回方向
    GET_EVENT_WHEEL,     // 返回有符号整型
};

struct MenuProperty
{
	float Cursor_X;		  // 当前光标位置X
	float Cursor_Y;		  // 当前光标位置Y
	float Cursor_W;		  // 当前光标尺寸宽
	float Cursor_H;		  // 当前光标尺寸高
	float Cursor_ActSpeed;		  // 光标动画速度系数;
	float Slide_ActSpeed;		  // 滚动动画速度系数;
	int8_t Font_Width;			  // 字体宽度
	int8_t Font_Height;			  // 字体宽度
	int8_t Line_Height;			  // 行高
	int8_t Layout_Margin;		  // 页边距

	int8_t Window_X;				// 窗口位置X
	int8_t Window_Y;                // 窗口位置Y
	uint8_t Window_W;               // 窗口宽度
	uint8_t Window_H;               // 窗口高度
};

int menu_command_callback(enum _menu_command command, ...);
void MENU_RunMenu(MENU_OptionTypeDef *OptionList);

int8_t Menu_Get_RollEvent(void);
int8_t Menu_Get_EnterEvent(void);
int8_t Menu_Get_BackEvent(void);

void MENU_RunMainMenu(void);
void MENU_RunSystemSetting(void);
void MENU_RunDisplaySetting(void);
void MENU_RunAnimation(void);

void Menu_cursorActStep(int8_t cursor_next);


#endif

