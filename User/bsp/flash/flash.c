#include "flash.h"

uint32_t address=0;
void flash_write(uint16_t* data ,uint8_t len)
{
    uint32_t SectorError;
    
    uint16_t i;
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef FLASH_EraseInitStructure;
    FLASH_EraseInitStructure.Banks = FLASH_BANK_1;
    FLASH_EraseInitStructure.NbPages = 1;
    FLASH_EraseInitStructure.PageAddress = WR_FLASH_START_ADR;
    FLASH_EraseInitStructure.TypeErase = FLASH_TYPEERASE_PAGES;
    if(HAL_FLASHEx_Erase(&FLASH_EraseInitStructure,&SectorError)!=HAL_OK)
    {
        while(1);
    }
    address=WR_FLASH_START_ADR;
    while(len--)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,address,*data);
        data++;
        address+=2;
    }
    HAL_FLASH_Lock();
}



void flash_read(uint16_t* data,uint8_t len)
{
    address = WR_FLASH_START_ADR;
    while (len--)
    {
        *data = *(__IO uint16_t*)address;
        data++;
        address+=2;
    }
    
}