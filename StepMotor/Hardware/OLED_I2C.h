#ifndef _OLED_I2C_H_
#define _OLED_I2C_H_
#endif
#include "stm32f1xx_hal.h"

#define Max_Column 128
#define Max_Row 64

#define OLED_CMD   		0x00
#define OLED_DATA 		0xFF

#define OLED_I2C_ADDRESS 0x78


void OLED_write_byte(uint8_t dat, uint8_t cmd);
void OLED_I2C_Init(void);
void OLED_I2C_Clear(void);
void OLED_I2C_ALLON(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Position(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);

uint32_t oled_pow(uint8_t m, uint8_t n);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Num_size,uint8_t scale);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char* BMP);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t*chr,uint8_t Char_Size);









