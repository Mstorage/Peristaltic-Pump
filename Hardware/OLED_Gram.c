#include "OLED_SPI.h"
#include "oledfont.h"
#include "usart.h"

unsigned char GRAM[SCREEN_HEIGHT / PAGE_HEIGHT][SCREEN_WIDTH] = {0};

/**
  * @brief  加载显存到屏幕;
  * @retval none
 */
void OLED_GRAMLODING(void)
{
	uint8_t page = 0;
	uint8_t colume = 0;
	
	for (page = 0; page < 8; page++)
	{
		for (colume = 0; colume < 128; colume++)
		{
            OLED_Set_Position(colume,page);
			OLED_write_byte(GRAM[page][colume],OLED_DATA);
		}
	}
}

/**
  * @brief  在目标区域清屏后显示位图;
  * @param  x_start:开始显示的x坐标;
  * @param	y_start:开始显示的y坐标;
  * @param	width:区域宽度；
  * @param	height:区域高度;

  * @retval none
 */
void OLED_GRAMShowImage(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height, const uint8_t *image)
{
    OLED_GRAMClearArea(x_start, y_start, width, height);
    uint8_t page_start = y_start / 8;
    uint8_t page_end =((y_start + height - 1) / 8) + 1;//必须向上取整，
    uint8_t offset = y_start % 8;

    for(uint8_t page_i = page_start; page_i < page_end; page_i ++)
    {
        for(uint8_t col = 0; col < width; col ++)
        {   
            if(page_i != page_start)//如果是最开始的字节不需要补全上一页未显示全的数据
            {   //将上一页没有存完的数据存到这一页
                GRAM[page_i][col + x_start] |= image[col + (page_i - page_start - 1) * width] >> (8 - offset);
            }

            if(offset != 0 && page_i == (page_end - 1)){continue;}   
            //将要存入的数据减去偏执后放入当前页当前列的显存字节
            GRAM[page_i][col + x_start] |= image[col + (page_i - page_start) * width] << offset;
            
            //printf("%d,GRAM:%X\n",page_i,GRAM[page_i][col + x_start]);
        }
    }

}

/**
  * @brief  目标清空区域内的显存位;
  * @param  x_start:开始清空的x坐标;
  * @param	y_start:开始清空的y坐标;
  * @param	width:区域宽度；
  * @param	height:区域高度;

  * @retval none
 */
void OLED_GRAMClearArea(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t heigh)
{
    uint8_t page, col;
	
	/*参数检查，保证指定区域不会超出屏幕范围*/
	if(x_start > 127){return;}
	if(y_start > 63){return;}

    for(page = y_start; page < y_start + heigh; page ++)
    {
        for(col = x_start; col < x_start + width;col++)
        {
            GRAM[y_start / 8][col] &= ~(0x01 << (page % 8));
        }
    }


}

/**
  * @brief  反转目标区域内的显存位;
  * @param  x_start:开始反转的x坐标;
  * @param	y_start:开始反转的y坐标;
  * @param	width:区域宽度；
  * @param	height:区域高度;

  * @retval none
 */
void OLED_GRAM_ReversArea(uint8_t x_start, uint8_t y_start, uint8_t width, uint8_t height) {
    	uint8_t i, j;
	
	/*参数检查，保证指定区域不会超出屏幕范围*/
	if (x_start > 127) {return;}
	if (y_start > 63) {return;}
	if (x_start + width > 128) {width = 128 - x_start;}
	if (y_start + height > 64) {height = 64 - y_start;}
	
	for (j = y_start; j < y_start + height; j ++)		//遍历指定页
	{
		for (i = x_start; i < x_start + width; i ++)	//遍历指定列
		{
			GRAM[j / 8][i] ^= 0x01 << (j % 8);	//将显存数组指定数据取反
		}
	}
}

/**
  * @brief  在显存任意位置加载点
  * @param  x:加载点的起始横坐标（0-128）;
  * @param	y:加载点的起始纵坐标（0-64）;
  * @retval none
 */
void OLED_GRAM_WritePoint(uint8_t point_x,uint8_t point_y)
{
    /*参数检查，保证指定位置不会超出屏幕范围*/
	if (point_x > 127) {return;}
	if (point_y > 63) {return;}

	uint8_t point_page = point_y / 8;
	uint8_t point_pixle = point_y % 8;
	GRAM[point_page][point_x] |= (0x01 << point_pixle);
}



/**
  * @brief  在任意位置加载字符
  * @param  x:加载字符串的起始横坐标（0-128）;
  * @param	y:加载字符串的起始纵坐标（0-64）;
  * @param	chr:想要加载的字符串地址；
  * @param	Char_Size:字符尺寸（8 / 16）;

  * @retval none
 */
void OLED_GRAM_ShowChar(uint8_t X, uint8_t Y, char Char, uint8_t FontSize)
{
	if (FontSize == 16)		//字体为宽8像素，高16像素
	{
		/*将ASCII字模库OLED_F8x16的指定数据以8*16的图像格式显示*/
		OLED_GRAMShowImage(X, Y, 8, 16, Chr_F8X16[Char - ' ']);
	}
	else if(FontSize == 8)	//字体为宽6像素，高8像素
	{
		/*将ASCII字模库OLED_F6x8的指定数据以6*8的图像格式显示*/
		OLED_GRAMShowImage(X, Y, 6, 8, Chr_F6x8[Char - ' ']);
	}
}

/**
  * @brief  在任意位置加载数字
  * @param  x:加载数字的起始横坐标（0-128）;
  * @param	y:加载数字串的起始纵坐标（0-64）;
  * @param	chr:想要加载的数字地址；
  * @param	Char_Size:字符尺寸（8 / 16）;

  * @retval none
 */
void OLED_GRAM_ShowNum(uint8_t x_start, uint8_t y_start, uint32_t Number, uint8_t Length, uint8_t FontSize,uint8_t gap)
{
	uint8_t i;
	for (i = 0; i < Length; i++)		//遍历数字的每一位							
	{
		/*调用OLED_ShowChar函数，依次显示每个数字*/

		/*Number / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
        uint8_t number_i = Number / oled_pow(10, Length - i - 1) % 10; 
		/*+ '0' 可将数字转换为字符格式*/
		OLED_GRAM_ShowChar(x_start + i * gap, y_start, number_i + '0', FontSize);
	}
}

/**
  * @brief  在任意位置加载字符串
  * @param  x:加载字符串的起始横坐标（0-128）;
  * @param	y:加载字符串的起始纵坐标（0-64）;
  * @param	chr:想要加载的字符串地址；
  * @param	Char_Size:字符尺寸（8 / 16）;

  * @retval none
 */
void OLED_GRAM_ShowString(uint8_t x_start, uint8_t y_start, uint8_t Fontsize, char *String)
{
    uint8_t i = 0;
    uint8_t x_index = x_start;
    uint8_t y_index = y_start;
    // uint8_t page_index = y_start / 8;
    uint8_t fontsize_width = (Fontsize == 8) ? 6 : 8;
    while(String[i] != '\0')
    {
        if(String[i] == '\n')
        {
            y_index += Fontsize;
            x_index = 0;
            i++;
        }
        if(x_index + fontsize_width > 128)
        {
            y_index += Fontsize;
            x_index = 0;
            i++;
        }
        if(String[i] > '~')
        {
            printf("no Chinese now\n");
        }else{
            OLED_GRAM_ShowChar(x_index, y_index, String[i], Fontsize);
            i++;
            x_index += fontsize_width;
        }


    }
}

/**
  * @brief  在任意位置加载字符串
  * @param  x:加载字符串的起始横坐标（0-128）;
  * @param	y:加载字符串的起始纵坐标（0-64）;
  * @param	chr:想要加载的字符串地址；
  * @param	Char_Size:字符尺寸（8 / 16）;

  * @retval none
 */
void OLED_GRAM_Printf(uint8_t X, uint8_t Y, uint8_t FontSize, char *format, ...)
{
	char String[30];						//定义字符数组
	va_list arg;							//定义可变参数列表数据类型的变量arg
	va_start(arg, format);					//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);			//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);							//结束变量arg
	OLED_GRAM_ShowString(X, Y, FontSize, String);//OLED显示字符数组（字符串）
}
