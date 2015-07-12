#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_chip.h"

#include "nand.h"


#define LED_PORT 	gpioPortC
#define LED_PIN0  	0
#define LED_PIN1  	1
#define LED_PIN2  	2
#define LED_PIN3  	3

 
volatile uint32_t g_msTicks; /* counts 1ms timeTicks */

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
  g_msTicks++;
}

void
Delay_ms(uint32_t ms)
{
  uint32_t c_time = g_msTicks;
  while (( g_msTicks - c_time) < ms);
}


uint8_t dataBuf[PHYSICAL_PAGE_SIZE];
NandID nandID;
/******************************************************************************
 * @brief  Main function
 * Main is called from _program_start, see assembly startup file
 *****************************************************************************/
int main(void)
{
    uint32_t i,j;
    uint16_t column;
    uint8_t page;
    uint8_t plane;
    uint16_t block;
 
    uint8_t status;
  
    /* Initialize chip */
    CHIP_Init();  
  
    SystemCoreClockUpdate();
  

    /* Setup SysTick Timer for 1 msec interrupts  */
 //   if (SysTick_Config(SystemCoreClock / 1000)) while (1) ;
  
    /* Enable clock for GPIO */
    CMU_ClockEnable(cmuClock_GPIO, true);
  
  
  

//    GPIO_PinModeSet(LED_PORT, LED_PIN0, gpioModePushPull, 0 );  
//    GPIO_PinModeSet(LED_PORT, LED_PIN1, gpioModePushPull, 0 ); 
//    GPIO_PinModeSet(LED_PORT, LED_PIN2, gpioModePushPull, 0 ); 
//    GPIO_PinModeSet(LED_PORT, LED_PIN3, gpioModePushPull, 0 ); 
  

    Nand_InitPins();

/*
    while (1)
    {
        GPIO_PortOutSetVal(LED_PORT, 0xf, 0xf);
        Delay_ms(1000);
        GPIO_PortOutSetVal(LED_PORT, 0x0, 0xf);
        Delay_ms(1000);
    }
*/

    Nand_Reset();
//    Delay_ms(500);
  
    page = 1;
    while (page--)
    {
        Nand_GetID(&nandID);
    }
    
    column = 0;
    page   = 0;
    plane  = 0;
    block  = 24;
    
    for (i = 0; i < PHYSICAL_PAGE_SIZE; i++)
    {
      dataBuf[i] = i & 0xFF;
    }
    
//    status = Nand_EraseBlock(page, plane, block);
    
//    if (status & 0x1) //if fail, you shall not pass here.
//    {
//        while(1);
//    }
    
//    status = Nand_ProgramPage(column, page, plane, block, dataBuf);
    
//    if (status & 0x1) //if fail, you shall not pass here.
//    {
//        while(1);
//    }
    
 //   Delay_ms(1500);    
    
    for (i = 0; i < PHYSICAL_PAGE_SIZE; i++)
    {
        dataBuf[i] = 0x0;
    }
 
    Nand_ReadPage(column, page, plane, block, dataBuf);
    
 //dammy bad block scan
 //cannot find any bad blocks???? is it possible???   
 /*   
    column = PAGE_SIZE;
    for (plane = 0; plane < PLANE_IN_DIE; plane++)
    {
        for (block = 0; block < PLANE_SIZE_BLOCKS; block++)
        {
            Nand_ReadPage(column, 0, plane, block, dataBuf); // check first page
            if (dataBuf[0] != 0xFF) while(1);
            Nand_ReadPage(column, (BLOCK_SIZE_PAGES -1), plane, block, dataBuf); //check last page
            if (dataBuf[0] != 0xFF) while(1);
        }
    }
 */
}



