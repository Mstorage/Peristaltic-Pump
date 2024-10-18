#ifndef _W25Q_H_
#define _W25Q_H_
#endif

#include "stm32f1xx_hal.h"
#include "spi.h"

#define FLASH_CS_Pin GPIO_PIN_12
#define FLASH_CS_GPIO_Port GPIOB

#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F

#define WAIT_Receive		    0xFF

#define Flash_CS_Clr()  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_RESET)//CS=0->Enable
#define Flash_CS_Set()  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_SET)//CS=1->Disable

void Flash_Write_Enabel(void);
uint8_t Flash_Read_SR(void);
uint8_t Flash_Write_SR(uint8_t status);
uint8_t Flash_WriteRead_Data(uint8_t txdata);
void Flash_Write_Data(uint8_t txdata);
uint16_t Flash_Read_ID(void);
void Flash_Erase_Sector(uint32_t Dst_Addr);
void Flash_Erase_Chip(void);
void Flash_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);
void Flash_SPI_SendAddress(uint32_t* Adr_Tran);
