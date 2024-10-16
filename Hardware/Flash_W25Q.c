#include "Flash_W25Q.h"

uint8_t W25Q128_BUFFER[4096];

/**
  * @brief  Flash使能函数;
  * @param  none
  * @retval none
  */
void Flash_Write_Enabel(void){
    Flash_CS_Set();
    Flash_CS_Clr();

    Flash_Write_Data(W25X_WriteEnable);
    Flash_CS_Set();
}

/**                                                                             
  * @brief  Flash读取状态寄存器值;
  * @param  none
  * @retval status:状态寄存器(Status Register)
  */
uint8_t Flash_Read_SR(void){
    uint8_t status = 0;
    uint8_t wait_receive = 0xFF;
    Flash_CS_Set();
    Flash_CS_Clr();

    Flash_Write_Data(W25X_WriteEnable);
    status = Flash_WriteRead_Data(wait_receive);
    Flash_CS_Set();
    return status;
}
/*
	状态寄存器(SR)
	BIT7  6   5   4   3   2   1   0
	SPR   RV  TB BP2 BP1 BP0 WEL BUSY
 	SPR:默认0,状态寄存器保护位,配合WP使用
 	TB,BP2,BP1,BP0:FLASH区域写保护设置
 	WEL:写使能锁定
 	BUSY:忙标记位(1,忙;0,空闲)
 	默认:0x00
*/
/**                                                                             
  * @brief  Flash写入状态寄存器值;
  * @param  status：需要写入的状态值;
  * @retval callbackRS:状态寄存器的返回值(现在我还不知道会返回个啥)
  */
uint8_t Flash_Write_SR(uint8_t status){
    uint8_t callbackRS = 0;
    Flash_CS_Set();
    Flash_CS_Clr();

    Flash_Write_Data(W25X_WriteStatusReg);
    callbackRS = Flash_WriteRead_Data(status);
    Flash_CS_Set();
    return callbackRS;
}

/**                                                                             
  * @brief  向Flash芯片发送数据并返回读取值;
  * @param  txdata: 向Flash芯片发送的8位数据;
  * @retval : SPI读取到的Flash返回值;
  */
uint8_t Flash_WriteRead_Data(uint8_t txdata){
    uint8_t rxdata = 0;
    HAL_SPI_TransmitReceive(&hspi2,&txdata,&rxdata, 1, 0x1000);
    return rxdata;
}

/**                                                                             
  * @brief  向Flash芯片发送数据;
  * @param  txdata: 向Flash芯片发送的8位数据;
  * @retval none;
  */
void Flash_Write_Data(uint8_t txdata){
    HAL_SPI_Transmit(&hspi2, &txdata, 1, 0x1000);
}

/**                                                                             
  * @brief  读取Flash芯片的ID型号;
  * @param  none
  * @retval :0xEF17,表示Flash芯片型号为W25Q128
  */
uint16_t Flash_Read_ID(void)
{
	uint16_t ID_receive = 0;	//声明ID返回变量
    uint8_t wait_receive = 0xFF;
    uint32_t ID_Adr = 0x00000000;
	Flash_CS_Set();
    Flash_CS_Clr();

    Flash_Write_Data(W25X_ManufactDeviceID); 
	
	/*发送24bit位的地址*/
    Flash_SPI_SendAddress(&ID_Adr);
	/*发无用字节,接收ID*/
    ID_receive |= Flash_WriteRead_Data(wait_receive) << 8;
    ID_receive |= Flash_WriteRead_Data(wait_receive);

	Flash_CS_Set();
	return ID_receive;	//返回ID
} 
/**                                                                             
  * @brief  擦除一个Flash芯片上的一个扇区;
  * @param  Dst_Addr: 需要擦除的扇区地址
  * @retval :返回擦除成功标志（这一块晚点再加）
  */
void Flash_Erase_Sector(uint32_t Dst_Addr)   
{  
 	Dst_Addr*=4096;		//一个扇区是4096个字节
    Flash_Write_Enabel();
    while((Flash_Read_SR()&0x01)==0x01); //等待BUSY位清空

  	Flash_CS_Set();
    Flash_CS_Clr();	//选中片选
    Flash_Write_Data(W25X_SectorErase);	//发送扇区擦除指令
	
	/*发送24bit地址*/
    Flash_SPI_SendAddress(&Dst_Addr);
		
	Flash_CS_Set();                              	      
    while((Flash_Read_SR()&0x01)==0x01); //等待BUSY位清空
}
/**                                                                             
  * @brief  擦除整个Flash芯片;
  * @param  none
  * @retval :返回擦除成功标志（这一块晚点再加）
  */
void Flash_Erase_Chip(void)   
{                                   
    Flash_Write_Enabel();
    while((Flash_Read_SR()&0x01)==0x01); //等待BUSY位清空
		
  	Flash_CS_Set();
    Flash_CS_Clr();	//选中片选
    Flash_WriteRead_Data(W25X_ChipErase);		//发送片擦除命令 
			
	Flash_CS_Set();                              	     	      
    while((Flash_Read_SR()&0x01)==0x01); 	//等待擦除完成
}

/** 
    * @brief  读取给定地址的固定数量数据
    * @param  pBuffer: 用于读取数据的缓存;
    * @param  ReadAddr: 所读数据的地址;
    * @param  NumByteToRead: 读取的数据大小(字节为单位，最大65535)
    * @retval :返回擦除成功标志（这一块晚点再加）
**/
void Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i;
	
	Flash_CS_Set();
    Flash_CS_Clr();	//选中片选                    	
    Flash_Write_Data(W25X_ReadData);	//发送读取命令
	
	Flash_SPI_SendAddress(&ReadAddr);
	/*开始发空指令,读取数据，返回寄存器的值*/
    for(i=0;i<NumByteToRead;i++)
	{    
		/*循环读数*/
        pBuffer[i]=Flash_WriteRead_Data(WAIT_Receive);   	 
    }
	Flash_CS_Set();		    	      
}  

/**
	* @brief  读取给定地址的1扇数据
    * @param  pBuffer: 要写的数据缓存;
    * @param  WriteAddr: 要写入的扇区地址（24bit）;
    * @param  NumByteToWrite: 要写入的数据大小(字节为单位，最大256)
    * @retval :返回写入成功标志（这一块晚点再加）
*/
void Flash_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
    Flash_Write_Enabel();	//写使能                  	

	Flash_CS_Set();  //选中片选
    Flash_CS_Clr();      
    Flash_Write_Data(W25X_PageProgram);	//发送写页命令
	
	/*发送24bit地址*/
    Flash_SPI_SendAddress(&WriteAddr);

	/*开始循环写数*/
    for(i=0;i<NumByteToWrite;i++){
		Flash_WriteRead_Data(pBuffer[i]);
	}

	Flash_CS_Set();	                            
    while((Flash_Read_SR()&0x01)==0x01); 	//等待擦除完成
} 

/**
	* @brief  按页写入,检查是否一页当中有存其他值,可以换页写入
    * @param  pBuffer: 要写的数据缓存;
    * @param  WriteAddr: 要写入的扇区地址（24bit）;
    * @param  NumByteToWrite: 要写入的数据大小(字节为单位，最大256)
    * @retval :返回写入成功标志（这一块晚点再加）
*/
void Flash_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	
	
	pageremain = 256 - WriteAddr % 256;	//当前地址下单页剩余的字节数
	if(NumByteToWrite <= pageremain) pageremain = NumByteToWrite;//不需要换页时将要写入字节数赋值给pageremain
	while(1)
	{	 
		Flash_Write_Page(pBuffer,WriteAddr, pageremain);	//写一页256个字节
		if(NumByteToWrite == pageremain)	//如果要写入的字节和单页剩余字数正好相等，写入结束
		{	/*写入结束*/
			break;	
		}
	 	else //如果要写入的字节>单页剩余字节，需要换页。
		{
			pBuffer += pageremain;		//写入数组=写入数组+剩余字节数
			WriteAddr += pageremain;		//写入地址=写入地址+剩余字节数，相当于下一页的开始地址

			NumByteToWrite -= pageremain;	//要写入的数据=要写入的数据-上一页已经写入的字节数
			/*一次可以写入256个字节*/
			if(NumByteToWrite > 256)	//如果要写入的数据还>256
			{
				pageremain = 256;		//单页剩余字节数=256
			} 
			else
			{	/*不够256个字节了*/
				pageremain = NumByteToWrite;	//要写入数据字节给当页剩余字节数
			}  	  
		}
	}	    
}

/**
 	* @brief  写数据(按扇区写入);
    * @param  pBuffer: 要写的数据缓存;
    * @param  WriteAddr: 要写入的地址;
    * @param  NumByteToWrite: 要写入的数据大小(字节为单位，最大65535)
    * @retval :返回写入成功标志（这一块晚点再加）
*/
void Flash_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25QXX_BUF;	  
   	W25QXX_BUF = W25Q128_BUFFER;	
	
 	secpos = WriteAddr / 4096;	//扇区地址
	secoff = WriteAddr % 4096;	//在扇区内的偏移
	secremain = 4096 - secoff;	//扇区剩余空间大小
	
	/*不大于4096个字节*/
	//如果（传入数量字节<扇区剩余空间大小） 将传入数量字节赋值给扇区剩余空间
 	if(NumByteToWrite <= secremain) secremain = NumByteToWrite;
	while(1) 
	{	/*读出整个扇区的内容*/
		Flash_Read(W25QXX_BUF, secpos * 4096,4096);
		/*校验数据*/
		for(i = 0; i < secremain; i++)
		{
			if(W25QXX_BUF[secoff + i] != 0XFF)
			{
				/*需要擦除 */
				break;
			}
				 	  
		}
		/*//需要擦除 */
		if(i < secremain)
		{
			/*擦除这个扇区*/
			Flash_Erase_Sector(secpos);		
			/*复制*/
			for(i = 0; i < secremain; i++)	   		
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			/*写入整个扇区*/
			Flash_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);
		}
		else
		{	/*写已经擦除了的,直接写入扇区剩余区间*/
			Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);
		}					   
		if(NumByteToWrite == secremain)
		{	/*写入结束了*/
			break;
		}
		else/*写入未结束*/
		{	/*扇区地址增1*/
			secpos ++;
			/*偏移位置为0*/
			secoff = 0; 	 
			/*指针偏移*/
		   	pBuffer += secremain;  				
			/*写地址偏移*/
			WriteAddr += secremain;				
			/*字节数递减*/
		   	NumByteToWrite -= secremain;
			/*下一个扇区还是写不完*/
			if(NumByteToWrite > 4096)
			{
				secremain = 4096;
			}
			else 
			{	/*下一个扇区可以写完了*/
				secremain = NumByteToWrite;
			}		
		}	 
	} 
}

/**
    * @brief  用于计算地址块和扇区的位置，并通过串口打印;
    * @param  address: 要获取块和扇区信息的地址;
    * @retval :返回成功标志（这一块晚点再加）
*/
void Get_Address_Analysis(uint32_t address)
{
	uint32_t addr = address;		//存储地址
	uint8_t block = addr>>16;	//23-16位是块的位置
	uint8_t sector = (addr<<16)>>28;	//15-12位是扇区的位置
	printf("addr:%x,block:%d,sector:%d\r\n",addr,block,sector);
}






/**
  * @brief  专用于向控制Flash的SPI外设发送地址信号,将24bit的数据分成3个8bit数据从高到低发送。
  * @param  Adr_Tran 需要写入的地址
  * @retval none
  */
// void Flash_SPI_SendAddress(uint32_t* Adr_Tran){
//     uint8_t* Adr_char = Adr_Tran;
//     HAL_SPI_Transmit(&hspi2, Adr_char + 1, 1, 0x1000);
//     HAL_SPI_Transmit(&hspi2, Adr_char + 2, 1, 0x1000);
//     HAL_SPI_Transmit(&hspi2, Adr_char + 3, 1, 0x1000);
// }
void Flash_SPI_SendAddress(uint32_t* Adr_Tran){
    uint8_t Adr_High = (uint8_t)(Adr_Tran[0]>>16);
    uint8_t Adr_Medi = (uint8_t)(Adr_Tran[0]>>8);
    uint8_t Adr_Low = (uint8_t)(Adr_Tran[0]);
    HAL_SPI_Transmit(&hspi2, &Adr_High, 1, 0x1000);
    HAL_SPI_Transmit(&hspi2, &Adr_Medi, 1, 0x1000);
    HAL_SPI_Transmit(&hspi2, &Adr_Low, 1, 0x1000);
}