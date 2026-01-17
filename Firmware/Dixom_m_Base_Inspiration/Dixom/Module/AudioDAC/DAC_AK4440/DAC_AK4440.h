#ifndef __AK4440_H__
#define __AK4440_H__
#include <stdint.h>

void             Init_DAC_AK4440(void);
void             Mute_DAC_AK4440(uint8_t state);

typedef struct
{
	uint8_t  ADDR;                   // Register address 0x00
	uint8_t  RSTN             :1;    // Internal Timing Reset
	uint8_t  NotUsed          :1;    // NotUsed
	uint8_t  DIF0             :1;    // Audio Data Interface Modes
	uint8_t  DIF1             :1;    // Audio Data Interface Modes
	uint8_t  DIF2             :1;    // Audio Data Interface Modes
	uint8_t  TDM0             :1;    // TDM Mode Select
	uint8_t  TDM1             :1;    // TDM Mode Select
	uint8_t  ACKS             :1;    // Master Clock Frequency Auto Setting Mode Enable (PCM only)
} tControl1_AK4440 ;

typedef struct
{
	uint8_t  ADDR;                   // Register address 0x01
	uint8_t  SMUTE            :1;    // Soft Mute Enable.
	uint8_t  DEM0             :1;    // De-emphasis Response
	uint8_t  DEM1             :1;    // De-emphasis Response
	uint8_t  DFS0             :1;    // DAC1 De-emphasis Response
	uint8_t  DFS1             :1;    // S Sampling speed control
	uint8_t  SLOW             :1;    // Slow Roll-off Filter Enable
	uint8_t  NotUsed          :1;    // NotUsed
	uint8_t  RRST             :1;    //  All registers are initialized.
} tControl2_AK4440 ;

typedef struct
{
	uint8_t  ADDR;                   // Register address 0x02
	uint8_t  PW1              :1;    // Power down control of DAC1
	uint8_t  PW2              :1;    // Power down control of DAC2
	uint8_t  PW3              :1;    // Power down control of DAC3
	uint8_t  PW4              :1;    // Power down control of DAC4
	uint8_t  NotUsed          :4;    // NotUsed
} tPwrDwnCon_AK4440;

typedef struct
{
	uint8_t  ADDR;                    // Register address0x05
	uint8_t  DEMD             :1;    // De-emphasis Enable bit of DAC1/2/3/4
	uint8_t  DEMC             :1;    // De-emphasis Enable bit of DAC1/2/3/4
	uint8_t  DEMB             :1;    // De-emphasis Enable bit of DAC1/2/3/4
	uint8_t  DEMA             :1;    // De-emphasis Enable bit of DAC1/2/3/4
	uint8_t  NotUsed          :4;    // NotUsed
} tDEMControl_AK4440  ;



typedef struct
{
	tControl1_AK4440 Control1;
	tControl2_AK4440 Control2;
	tPwrDwnCon_AK4440 PwrDwnCon;
	tDEMControl_AK4440 DEMControl;
} sDAC_AK4440;



#endif
