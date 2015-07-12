#ifndef DEF_PINS_H
#define DEF_PINS_H

#include "em_gpio.h"

/*************************************/
/********NAND specifiec pins**********/
/*************************************/

/*ADDRESS LATCH ENABLE -> OUTPUT*/
#define ALE_PORT    gpioPortE
#define ALE_PIN     3

/*COMMAND LATCH ENABLE -> OUTPUT*/
#define CLE_PORT    gpioPortE
#define CLE_PIN     2

/*WRITE PROTECT -> OUTPUT*/
#define WP_PORT     gpioPortC
#define WP_PIN      12

/*WRITE ENABLE -> OUTPUT*/
#define WE_PORT     gpioPortC
#define WE_PIN      14

/*READ ENABLE -> OUTPUT*/
#define RE_PORT     gpioPortE
#define RE_PIN      0

/*CHIP ENABLE -> OUTPUT*/
#define CE_PORT     gpioPortE
#define CE_PIN      1

/*READY BUSY <- INPUT*/
#define R_B_PORT    gpioPortC
#define R_B_PIN     7

/*DATA INPUTS/OUTPUTS*/
#define DATA_PORT   gpioPortD
#define DATA_MASK   0xFF

/******************************************/
/*****Useful NAND specifiec macroses*******/
/******************************************/

#define SET_ALE()    do {GPIO_PinOutSet(ALE_PORT, ALE_PIN);} while(0)
#define SET_CLE()    do {GPIO_PinOutSet(CLE_PORT, CLE_PIN);} while(0)
#define SET_WP()     do {GPIO_PinOutSet(WP_PORT, WP_PIN);} while(0)
#define SET_WE()     do {GPIO_PinOutSet(WE_PORT, WE_PIN);} while(0)
#define SET_RE()     do {GPIO_PinOutSet(RE_PORT, RE_PIN);} while(0)
#define SET_CE()     do {GPIO_PinOutSet(CE_PORT, CE_PIN);} while(0)

#define CLEAR_ALE()  do {GPIO_PinOutClear(ALE_PORT, ALE_PIN);} while(0)
#define CLEAR_CLE()  do {GPIO_PinOutClear(CLE_PORT, CLE_PIN);} while(0)
#define CLEAR_WP()   do {GPIO_PinOutClear(WP_PORT, WP_PIN);} while(0)
#define CLEAR_WE()   do {GPIO_PinOutClear(WE_PORT, WE_PIN);} while(0)
#define CLEAR_RE()   do {GPIO_PinOutClear(RE_PORT, RE_PIN);} while(0)
#define CLEAR_CE()   do {GPIO_PinOutClear(CE_PORT, CE_PIN);} while(0)

#define GET_R_B()    (GPIO_PinInGet(R_B_PORT, R_B_PIN))
                      
enum PORT_MODE
{
  INPUT = 0,
  OUTPUT,
};

 __STATIC_INLINE void ConfigDataPort(uint8_t mode)
{
    uint8_t data_pin;
    
    if (mode == INPUT)
    {
        for (data_pin = 0; data_pin < 8; data_pin++) //set data pins as input, can't find API for port config
            GPIO_PinModeSet(DATA_PORT, data_pin, gpioModeInput, 0 );
    }
    else if (mode == OUTPUT)
    {
        for (data_pin = 0; data_pin < 8; data_pin++) //set data pins as output, can't find API for port config
            GPIO_PinModeSet(DATA_PORT, data_pin, gpioModePushPull, 0 );
    }
}

 __STATIC_INLINE void SET_DATA(uint8_t data)
{   
    GPIO_PortOutSetVal(DATA_PORT, data, DATA_MASK); 
}

 __STATIC_INLINE uint32_t GET_DATA()
{    
    return (GPIO_PortInGet(DATA_PORT) & DATA_MASK);
}
#endif //DEF_PINS_H
