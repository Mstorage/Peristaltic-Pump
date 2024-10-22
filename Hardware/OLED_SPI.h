#ifndef _OLED_SPI_H_
#define _OLED_SPI_H_
#include "stm32f1xx_hal.h"

#include <stdarg.h>
#include <string.h>

#define Max_Column 128
#define Max_Row 64

#define OLED_DATA 		0xFF
#define OLED_CMD  		0x00

#define OLED_I2C_ADDRESS 0x78

#define FONT_SIZE_8X6 8
#define FONT_SIZE_16X8 16

#define OLED_DC_Clr() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)//DC=0->CMD
#define OLED_DC_Set() HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)//DC=1->DATA

#define OLED_CS_Clr()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET)//CS=0->Enable
#define OLED_CS_Set()  HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_SET)//CS=1->Disable

#define OLED_RST_Clr() HAL_GPIO_WritePin(OLED_RST_GPIO_Port,OLED_RST_Pin,GPIO_PIN_RESET)//RES
#define OLED_RST_Set() HAL_GPIO_WritePin(OLED_RST_GPIO_Port,OLED_RST_Pin,GPIO_PIN_SET)


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PAGE_HEIGHT 8

void OLED_write_byte(uint8_t dat, uint8_t cmd);
uint8_t OLED_Init(void);
void OLED_Clear(void);
void OLED_ALLON(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Position(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);

uint32_t oled_pow(uint8_t m, uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Num_size);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char* BMP);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t*chr,uint8_t Char_Size);

void OLED_GRAMLODING(void);
void OLED_GRAM_ReversArea(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height);
void OLED_GRAM_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize);
void OLED_GRAM_WritePoint(uint8_t point_x,uint8_t point_y);
void OLED_GRAMShowImage(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height, const uint8_t *image);
void OLED_GRAMClearArea(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t heigh);
void OLED_GRAM_WritePoint(uint8_t point_x,uint8_t point_y);
void OLED_GRAM_ShowNum(uint8_t x_start, uint8_t y_start, uint32_t Number, uint8_t Length, uint8_t FontSize, uint8_t gap);
void OLED_GRAM_ShowString(uint8_t x_start, uint8_t y_start, uint8_t Fontsize, char *String);
void OLED_GRAM_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...);

extern unsigned char GRAM[SCREEN_HEIGHT / PAGE_HEIGHT][SCREEN_WIDTH];

#endif









