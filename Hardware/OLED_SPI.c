#include "OLED_SPI.h"
#include "oledfont.h"
#include "spi.h"


/**
  * @brief  写入数据或是命令到OLED屏幕;
  * @param  dat: 要写入的数据
  * @param  cmd: OLED_CMD 代表后面写入的字节是指令; OLED_DATA 代表后面写入的字节是数据;
  * @retval none
  */
void OLED_write_byte(uint8_t txdat, uint8_t data_cmd)
{
	//uint8_t rxdat = 0;
	OLED_CS_Clr();
	data_cmd ? OLED_DC_Set() : OLED_DC_Clr();
	HAL_SPI_Transmit(&hspi1,&txdat,1,1000);
	OLED_CS_Set();
	OLED_DC_Set();
}

/**
  * @brief  初始化OLED屏幕;
  * @param  none
  * @retval none
  */
uint8_t OLED_Init(void)			//端口初始化
{
	OLED_RST_Set();
	HAL_Delay(200);
	OLED_RST_Clr();
	HAL_Delay(200);
	OLED_RST_Set();

	OLED_write_byte(0xAE, OLED_CMD); //--turn off oled panel
	OLED_write_byte(0x00, OLED_CMD); //---set low column address
	OLED_write_byte(0x10, OLED_CMD); //---set high column address
	OLED_write_byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_write_byte(0x81, OLED_CMD); //--set contrast control register
	OLED_write_byte(0xFF, OLED_CMD); // Set SEG Output Current Brightness

	OLED_write_byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xA0Colum 0xa1normal
	OLED_write_byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0Row 0xc8normal

	OLED_write_byte(0xA6, OLED_CMD); //--set normal display (A6h normal A7h reversal)

	OLED_write_byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_write_byte(0x3f, OLED_CMD); //--1/64 duty

	OLED_write_byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_write_byte(0x00, OLED_CMD); //-not offset

	OLED_write_byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	OLED_write_byte(0xc0, OLED_CMD); //--set divide ratio, 0x10-0xF0

	OLED_write_byte(0xD9, OLED_CMD); //--set pre-charge period
	OLED_write_byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock

	OLED_write_byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	OLED_write_byte(0x12, OLED_CMD);

	OLED_write_byte(0xDB, OLED_CMD); //--set vcomh
	OLED_write_byte(0x40, OLED_CMD); // Set VCOM Deselect Level

	OLED_write_byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_write_byte(0x02, OLED_CMD); //

	OLED_write_byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	OLED_write_byte(0x14, OLED_CMD); //--set(0x10) disable

	OLED_write_byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5) 0xa4 gram 0xa5 allon
	OLED_write_byte(0xAF, OLED_CMD); //--turn on oled panel

	OLED_Clear();
	OLED_Set_Position(0,0);
	return HAL_OK;
}

/**
  * @brief  清空屏幕，所有地址数据赋0;
  * @param  none
  * @retval none
  */
void OLED_Clear(void)
{
	uint8_t i,j;
	for(i = 0;i < 8;i++)
	{
		OLED_write_byte(0xb0 + i, OLED_CMD);	//设置页地址（0~7）
		OLED_write_byte(0x00, OLED_CMD);		//设置显示位置—列低地址
		OLED_write_byte(0x10, OLED_CMD);		//设置显示位置—列高地址
		for(j = 0;j < 128;j++)
		{
			OLED_write_byte(0, OLED_DATA);
		}
	}
}

/**
  * @brief  清空屏幕，所有地址数据赋1;
  * @param  none
  * @retval none
  */
void OLED_ALLON(void)
{
	uint8_t i,j;
	for(i = 0;i < 8;i++)
	{
		OLED_write_byte(0xb0 + i, OLED_CMD);	//设置行地址（0~7）
		OLED_write_byte(0x00, OLED_CMD);		//设置显示位置—列低地址
		OLED_write_byte(0x10, OLED_CMD);		//设置显示位置—列高地址
		for(j = 0;j < 128;j++)
		{
			OLED_write_byte(0xFF, OLED_DATA);
		}
	}
}

/**
  * @brief  开启OLED显示;
  * @param  none
  * @retval none
  */
void OLED_Display_On(void)
{
	OLED_write_byte(0x8D, OLED_CMD);	// SET DCDC COMMAND
	OLED_write_byte(0x14, OLED_CMD);	//DCDC OFF
	OLED_write_byte(0xAF, OLED_CMD);	//DISPLAY ON
}

/**
  * @brief  关闭OLED显示;
  * @param  none
  * @retval none
  */
void OLED_Display_Off(void)
{
	OLED_write_byte(0X8D,OLED_CMD);  //SET DCDC COMMAND
	OLED_write_byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_write_byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   		

/**
  * @brief  坐标设置，设置屏幕光标位置;
  * @param  x:设置的横坐标（x < 128）
  * @param  y:设置的纵坐标（y < 8）
  * @retval none
  */
void OLED_Set_Position(uint8_t x, uint8_t y)
{ 
	OLED_write_byte(0xb0 + y, OLED_CMD);
	OLED_write_byte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);
	OLED_write_byte((x & 0x0F), OLED_CMD); 
}

/**
  * @brief  显示字符;
  
  * @param  x:显示字符的起始横坐标（x < 127）;
  * @param	y:显示字符的起始纵坐标;(y < 63)
  * @param	chr:要显示的字符;
  * @param	char_size:选择字体大小（16 / 8）;

  * @retval none
 */
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{      	
	unsigned char c = 0, i = 0;	
		c = chr - ' ';	//得到偏移后的值			
		if(x > Max_Column - 1)	//当x大于最大列数时
		{
			x = 0;				//从新的一行开始
			y = y + Char_Size / 8;			//下移一列
		}
		if(Char_Size == 16)
		{
			OLED_Set_Position(x, y);	
			for(i = 0;i < 8;i++)
			{
				OLED_write_byte(Chr_F8X16[c*16][i],OLED_DATA);
			}
			OLED_Set_Position(x, y+1);
			for(i = 0; i < 8; i++)
			{
				OLED_write_byte(Chr_F8X16[c*16][i+8], OLED_DATA);
			}
		}
			else {	
				OLED_Set_Position(x,y);
				for(i=0;i<6;i++)
				OLED_write_byte(Chr_F6x8[c][i],OLED_DATA);
			}
}

/**
  * @brief  计算m*n,得到m进制下最大的第n位;
  * @param  m:进制
  * @param  n:最大位数（x < 64）
  * @retval result:m进制下最大那一位代表的数;
  */
uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1 ;	 
	while(n--){
		result *= m;
	}    
	return result;
}	

/**
  * @brief  显示数字;
  
  * @param  x:显示数字的起始横坐标(0~127);
  * @param	y:显示数字的起始纵坐标(0~7);
  * @param	num:要显示的数字;
  * @param	len:要显示的数字长度;
  * @param	Num_size:NUM_SIZE_8或者NUM_SIZE_16;

  * @retval none
 */
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t Num_size)
{         	
	uint8_t place_now = 0, number_now = 0;		//表示当前正操作（输出数）第几位
	//uint8_t compelet_flag = 0;		//标志位，为0时表示有效数字还未显示完毕，为1时表示有效数字显示完毕;
	uint8_t len_t = len - 1;
	uint8_t Num_size_t = 0;

	switch (Num_size) {
			case 8:
				Num_size_t = 6;
				break;
			case 16:
				Num_size_t = 8;
				break;
			default:
				Num_size_t = 8;
		}
	for(place_now = 0; place_now < len; place_now++){
		number_now = (num / oled_pow(10, len_t - place_now)) % 10;
		// if(place_now < (len_t) && compelet_flag == 0)
		// {
		// 	if(place_now == 0)
		// 	{
		// 		OLED_ShowChar(x + (Num_size_t) * place_now, y, '0', Num_size);
		// 		continue;
		// 	}else compelet_flag = 1;  
		// }
		OLED_ShowChar(x + (Num_size_t) * place_now, y, number_now + '0', Num_size); 
	}
} 

/**
  * @brief  加载BMP格式图片;
  * @param  x0:加载图片的起始横坐标;
  * @param	x1:图片结束横坐标	(x <= 128);
  * @param	y0:图片起始纵坐标;
  * @param	y1:图片结束纵坐标	(y <= 8);
  * @param	BMP_index:需要加载的BMP图片地址;

  * @retval none
 */
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char* BMP_index)
{ 	
 unsigned int j = 0;
 unsigned char x, y;
  
	for(y = y0; y < y1; y++)		//再遍历行
	{
		x = x1;
		while(x > x0)
		{
			OLED_Set_Position(x, y);
	    	OLED_write_byte(BMP_index[j++], OLED_DATA);
			x = x - 1;
		}
	}
} 



void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t, adder = 0;	//这个adder好像没什么卵用？
	OLED_Set_Position(x, y);	
    for(t = 0; t < 16; t++)
	{
		OLED_write_byte(Chr_Chinese[2*no][t],OLED_DATA);
		adder += 1;
     }	
		OLED_Set_Position(x, y+1);	
    for(t = 0; t < 16; t++)
	{	
		OLED_write_byte(Chr_Chinese[2*no+1][t],OLED_DATA);
		adder += 1;
    }					
}

/**
  * @brief  加载字符串;
  * @param  x:加载字符串的起始横坐标;
  * @param	y:加载字符串的起始纵坐标;
  * @param	chr:想要加载的字符串地址；
  * @param	Char_Size:字符尺寸（8 / 16）;

  * @retval none
 */
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t*chr,uint8_t Char_Size)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{		
		OLED_ShowChar(x, y, chr[j], Char_Size);
		x += 8;
		if(x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}


