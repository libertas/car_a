#include "flash.h"
#include "usart.h" 


uint32_t flread(uint32_t faddr)
{
	return *(uint32_t*)faddr; 
}  


uint16_t flash_get_sector(uint32_t addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}



void flwriten(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	uint32_t addrx=0;
	uint32_t endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)
	  return;
	FLASH_Unlock();
  FLASH_DataCacheCmd(DISABLE);
 		
	addrx=WriteAddr;
	endaddr=WriteAddr+NumToWrite*4;
	if(addrx<0X1FFF0000)
	{
		while(addrx<endaddr)
		{
			if(flread(addrx)!=0XFFFFFFFF)
			{   
				status=FLASH_EraseSector(flash_get_sector(addrx),VoltageRange_3);
				if(status!=FLASH_COMPLETE)
					break;
			}else addrx+=4;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)
			{ 
				break;
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);
	FLASH_Lock();
} 


void flreadn(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead)   	
{
	uint32_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=flread(ReadAddr);
		ReadAddr+=4;	
	}
}
