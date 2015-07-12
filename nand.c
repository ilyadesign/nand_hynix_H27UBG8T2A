
#include "nand.h"

void Nand_InitPins(void)
{
    GPIO_PinModeSet(CE_PORT, CE_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(ALE_PORT, ALE_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(CLE_PORT, CLE_PIN, gpioModePushPull, 0);
    GPIO_PinModeSet(WE_PORT, WE_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(RE_PORT, RE_PIN, gpioModePushPull, 1);
    GPIO_PinModeSet(WP_PORT, WP_PIN, gpioModePushPull, 1);
    
    GPIO_PinModeSet(R_B_PORT, R_B_PIN, gpioModeInput, 0);
}

/************************************/
/************************************/
void Nand_GetID(NandID * idBuf)
{   
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_READ_ID);
    Nand_SendAdressByte(0x00);
    Nand_ReadData((uint8_t *)idBuf, sizeof(NandID));
    
    SET_CE(); //disable NAND
}

/************************************/
/************************************/
void Nand_Reset(void)
{  
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_RESET);
    
    SET_CE(); //disable NAND
}

/************************************/
/************************************/
uint8_t Nand_ReadStatusReg(void)
{  
    uint8_t status;
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_READ_STATUS_REGISTER);
    Nand_ReadData(&status, sizeof(status));
    
    SET_CE(); //disable NAND
    return status;
}

/************************************/
/************************************/
void Nand_ReadPage(uint16_t column, uint8_t page, uint8_t plane, uint16_t block, uint8_t * dataBuf)
{   
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_PAGE_READ_1CYCL);
    Nand_SendAdress(column, page, plane, block);
    Nand_SendCommand(CMD_PAGE_READ_2CYCL);
    
    //TODO: read page size from NAND ID
    //TODO: use DMA to read???
    Nand_ReadData(dataBuf, (PHYSICAL_PAGE_SIZE - column));
    
    SET_CE(); //disable NAND
}

/************************************/
/************************************/
uint8_t Nand_ProgramPage(uint16_t column, uint8_t page, uint8_t plane, uint16_t block, uint8_t * dataBuf)
{
    uint8_t status;
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_PAGE_PROGRAM_1CYCL);
    Nand_SendAdress(column, page, plane, block);
    Nand_WriteData(dataBuf, (PHYSICAL_PAGE_SIZE - column));   
    Nand_SendCommand(CMD_PAGE_PROGRAM_2CYCL);
    
    status = Nand_ReadStatusReg();
    SET_CE(); //disable NAND
    return status;
}


/************************************/
/************************************/
uint8_t Nand_EraseBlock(uint8_t plane, uint16_t block)
{
    uint8_t status;
    CLEAR_CE(); //enable nand
    
    Nand_SendCommand(CMD_ERASE_BLOCK_1CYCL);
    Nand_SendRawAdress(0x0, plane, block);
    Nand_SendCommand(CMD_ERASE_BLOCK_2CYCL);
    
    status = Nand_ReadStatusReg();
    SET_CE(); //disable NAND
    return status;
}
