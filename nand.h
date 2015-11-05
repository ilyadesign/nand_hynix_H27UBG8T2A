#ifndef NAND_H
#define NAND_H

#include "def_pins.h"
#include "stdint.h"

#define CMD_PAGE_READ_1CYCL             0x00
#define CMD_PAGE_READ_2CYCL             0x30
#define CMD_READ_STATUS_REGISTER        0x70
#define CMD_PAGE_PROGRAM_1CYCL          0x80
#define CMD_PAGE_PROGRAM_2CYCL          0x10
#define CMD_ERASE_BLOCK_1CYCL           0x60
#define CMD_ERASE_BLOCK_2CYCL           0xD0
#define CMD_READ_ID                     0x90
#define CMD_RESET                       0xFF
#define CMD_RANDOM_DATA_OUTPUT_1CYCL    0x05
#define CMD_RANDOM_DATA_OUTPUT_2CYCL    0xE0
#define CMD_READ_PARAM_PAGE             0xEC




//hynix H27UBG8T2ATR
#define   PAGE_SIZE           8192
#define   SPARE_AREA          448
#define   PHYSICAL_PAGE_SIZE  (PAGE_SIZE + SPARE_AREA)
#define   BLOCK_SIZE_PAGES    256
#define   PLANE_SIZE_BLOCKS   1024
#define   PLANE_IN_DIE         2
#define   TOTAL_BLOKS         (PLANE_IN_DIE * PLANE_SIZE_BLOCKS)



extern uint8_t dataBuf[PHYSICAL_PAGE_SIZE];


//hynix H27UBG8T2ATR ID: 
//  1   0xAD   manufucture hynix 
//  2   0xD7   device code 
//  3   0x94 
//  4   0x9A
//  5   0x74 
//  6   0x42
typedef struct
{
    uint8_t manufactureID;
    uint8_t deviceID;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4; 
}NandID;

typedef struct 
{
	/* rev info and features block */
	/* 'O' 'N' 'F' 'I'  */
	uint8_t   sig[4];
	uint16_t  revision;
	uint16_t  features;
	uint16_t  opt_cmd;
	uint8_t   reserved0[2];
	uint16_t  ext_param_page_length; /* since ONFI 2.1 */
	uint8_t   num_of_param_pages;        /* since ONFI 2.1 */
	uint8_t   reserved1[17];

	/* manufacturer information block */
	char      manufacturer[12];
	char      model[20];
	uint8_t   jedec_id;
	uint16_t  date_code;
	uint8_t   reserved2[13];

	/* memory organization block */
	uint32_t  byte_per_page;
	uint16_t  spare_bytes_per_page;
	uint32_t  data_bytes_per_ppage;
	uint16_t  spare_bytes_per_ppage;
	uint32_t  pages_per_block;
	uint32_t  blocks_per_lun;
	uint8_t   lun_count;
	uint8_t   addr_cycles;
	uint8_t   bits_per_cell;
	uint16_t  bb_per_lun;
	uint16_t  block_endurance;
	uint8_t   guaranteed_good_blocks;
	uint16_t  guaranteed_block_endurance;
	uint8_t   programs_per_page;
	uint8_t   ppage_attr;
	uint8_t   ecc_bits;
	uint8_t   interleaved_bits;
	uint8_t   interleaved_ops;
	uint8_t   reserved3[13];

	/* electrical parameter block */
	uint8_t   io_pin_capacitance_max;
	uint16_t  async_timing_mode;
	uint16_t  program_cache_timing_mode;
	uint16_t  t_prog;
	uint16_t  t_bers;
	uint16_t  t_r;
	uint16_t  t_ccs;
	uint16_t  src_sync_timing_mode;
	uint16_t  src_ssync_features;
	uint16_t  clk_pin_capacitance_typ;
	uint16_t  io_pin_capacitance_typ;
	uint16_t  input_pin_capacitance_typ;
	uint8_t   input_pin_capacitance_max;
	uint8_t   driver_strength_support;
	uint16_t  t_int_r;
	uint16_t  t_ald;
	uint8_t   reserved4[7];

	/* vendor */
	uint16_t  vendor_revision;
	uint8_t   vendor[88];

	uint16_t  crc;
}NandParams; //only for ONFI, I stoll it from linux.


__STATIC_INLINE void Nand_SendCommand(uint8_t cmd)
{
    ConfigDataPort(OUTPUT);
    CLEAR_WE();
    SET_CLE();   //enable command latch
    SET_DATA(cmd); // put data on data bus
    SET_WE();    // do write
    CLEAR_CLE();
}

__STATIC_INLINE void Nand_SendAdressByte(uint8_t adr)
{
    ConfigDataPort(OUTPUT);
    CLEAR_WE();
    SET_ALE(); //enable adress latch
    SET_DATA(adr); // put data on data bus
    SET_WE(); // do write
    CLEAR_ALE();  
}
 
__STATIC_INLINE void Nand_SendAdress(uint16_t column, uint8_t page, uint8_t plane, uint16_t block)
{
   //TODO: add MASK's and macrocess?
    Nand_SendAdressByte(column & 0xFF);
    Nand_SendAdressByte((column >> 8) & 0x3F);
    Nand_SendAdressByte(page);
    Nand_SendAdressByte(((plane & 1) | (block << 1)) & 0xFF);
    Nand_SendAdressByte((block >> 7) & 0x07);
}
 
__STATIC_INLINE void Nand_SendRawAdress(uint8_t page, uint8_t plane, uint16_t block)
{
   //TODO: add MASK's and macrocess?
    Nand_SendAdressByte(page);
    Nand_SendAdressByte(((plane & 1) | (block << 1)) & 0xFF);
    Nand_SendAdressByte((block >> 7) & 0x07);
}
 
__STATIC_INLINE void Nand_ReadData(uint8_t * pBuf, uint32_t bytes)
{
    uint32_t cycles;
    
    ConfigDataPort(INPUT); 
    
    while (!GET_R_B())
      ;

    for (cycles = 0; cycles < bytes; cycles++)
    {
        CLEAR_RE();
        *pBuf++ = GET_DATA();
        SET_RE();
    }
}

__STATIC_INLINE uint8_t Nand_ReadByte(void)
{  
    uint8_t tmp;
    ConfigDataPort(INPUT); 
    
    while (!GET_R_B())
      ;

    CLEAR_RE();
    tmp = GET_DATA();
    SET_RE();
    return tmp;
}
 
__STATIC_INLINE void Nand_WriteData(uint8_t * pBuf, uint32_t bytes)
{
    uint32_t cycles;
    
    ConfigDataPort(OUTPUT); 
    
    for (cycles = 0; cycles < bytes; cycles++)
    {
        SET_DATA(*pBuf++);
        CLEAR_WE();
        SET_WE();
    }
    
    while (!GET_R_B())
        ;
}
 


void Nand_InitPins(void);
void Nand_Reset(void);
void Nand_GetID(NandID * idBuf);
bool Nand_DetectONFI(void);
uint8_t Nand_ReadStatusReg(void);
void Nand_ReadPage(uint16_t column, uint8_t page, uint8_t plane, uint16_t block, uint8_t * dataBuf);
uint8_t Nand_ProgramPage(uint16_t column, uint8_t page, uint8_t plane, uint16_t block, uint8_t * dataBuf);
uint8_t Nand_EraseBlock(uint8_t plane, uint16_t block);
void Nand_BadBlockScan(uint8_t * badBlockMap);
 
#endif //NAND_H
