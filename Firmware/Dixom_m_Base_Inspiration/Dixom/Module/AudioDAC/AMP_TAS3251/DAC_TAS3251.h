#ifndef __TAS3251_H__
#define __TAS3251_H__
#include <stdint.h>


void             Init_DAC_TAS3251(void);
void             Mute_DAC_TAS3251(uint8_t state);
void             Volume_DAC_TAS3251(uint8_t *pData);




#define   TAS3251_CHANGE_PAGE_REG          0x00
#define   TAS3251_CHANGE_BOOK_REG          0x7F
#define   TAS3251_DSP_BOOK                 0x8C
#define   TAS3251_DSP_SWAP_FLAG_PAGE       0x23
#define   TAS3251_DSP_SWAP_REG             0x14
#define   TAS3251_OPERATION_MODES_REG      0x02
#define   TAS3251_MUTE_REG                 0x03
#define   TAS3251_MUTE_ENABLE              0x11
#define   TAS3251_MUTE_DISABLE             0x00
#define   TAS3251_LEFT_VOLUME_REG          0x3D
#define   TAS3251_RIGHT_VOLUME_REG         0x3E

typedef struct
{
	uint8_t  ADDR;                   // Register address 0x01
	uint8_t  RSTR             :1;    // Reset Registers 0: Normal 1: Reset mode registers
	uint8_t  Reserved1        :3;    // Reserved
	uint8_t  RSTM             :1;    // Registers 0: Normal 1: Reset modules
	uint8_t  Reserved2        :3;    // Audio Data Interface Modes
} tRegister1 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x02
	uint8_t  RQPD              :1;    // Powerdown Request 0: Normal operation 1: Powerdown mode
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  RQST              :1;    // Standby Request 0: Normal operation 1: Standby mode
	uint8_t  Reserved2         :2;    // Reserved
	uint8_t  DSPR              :1;    // DSP reset 0: Normal operation 1: Reset the DSP
} tRegister2 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x03
	uint8_t  RQMR              :1;    // Mute Right Channel 0: Normal volume 1: Mute
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  RQML              :1;    // Mute Left Channel 0: Normal volume 1: Mute
	uint8_t  Reserved2         :3;    // Reserved
} tRegister3 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x04
	uint8_t  PLLE              :1;    // PLL Enable 0: Disable PLL 1: Enable PLL
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  PLCK              :1;    // PLL Lock Flag 0: The PLL is locked 1: The PLL is not locked
	uint8_t  Reserved2         :3;    // Reserved
} tRegister4 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x06
	uint8_t  Reserved1         :3;    // PLL Enable 0: Disable PLL 1: Enable PLL
	uint8_t  DBPG              :1;    // Page auto increment disable 0: Enable Page auto increment 1: Disable Page auto increment
	uint8_t  Reserved2         :4;    // Reserved
} tRegister6 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x07
	uint8_t  SDSL              :1;    // SDOUT Select 0: SDOUT is the DSP output (post-processing) 1: SDOUT is the DSP input (pre-processing)
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  DEMP              :1;    // De-Emphasis Enable 0: De-emphasis filter is disabled 	1: De-emphasis filter is enabled
	uint8_t  Reserved2         :3;    // Reserved
} tRegister7 ;


typedef struct
{
	uint8_t  ADDR;                    // Register address 0x08
	uint8_t  Reserved1         :2;    // Reserved
	uint8_t  G2OE              :1;    // SDOUT Output Enable
	uint8_t  MUTEOE            :1;    // 0: SDOUT is input 1: SDOUT is output
	uint8_t  Reserved2         :4;    // Reserved
} tRegister8 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x09
	uint8_t  LRKO              :1;    // LRCLK Output Enable 0: LRCLK is input (I2S slave mode) 1: LRCLK is output (I2S master mode)
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  SCLKO             :1;    // SCLK Output Enable  0: SCLK is input (I2S slave mode) 1: SCLK is output (I2S master mode)
	uint8_t  SCLKP             :1;    // SCLK Polarity 0: Normal SCLK mode 1: Inverted SCLK mode
	uint8_t  Reserved2         :2;    // Reserved
} tRegister9 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x12
	uint8_t  RLRK              :1;    // Master Mode LRCLK Divider Reset 0: Master mode LRCLK clock divider is reset 1: Master mode LRCLK clock divider is functional
	uint8_t  RSCLK             :1;    // Master Mode SCLK Divider Reset  0: Master mode SCLK clock divider is reset 1: Master mode SCLK clock divider is functional
	uint8_t  Reserved          :6;    // Reserved
} tRegister12 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0D
	uint8_t  SDSP              :3;    // DAC clock source 001: PLL clock 010: OSC clock 011: MCLK clock 100: SCLK clock 101: GPIO (selected using P0-R16)
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  SREF              :1;    // DSP clock source 0: The PLL reference clock is MCLK 1: The PLL reference clock is SCLK
	uint8_t  Reserved2         :3;    // Reserved
} tRegister13 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0E
	uint8_t  SOSR              :3;    // OSR clock source 000: DAC clock 001: Master clock 010: PLL clock 011: OSC clock 100: MCLK clock 101: SCLK clock 110: GPIO
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  SDAC              :3;    // DAC clock source 000: Master clock 001: PLL clock 011: MCLK clock 100: SCLK clock 101: GPIO
	uint8_t  Reserved2         :1;    // Reserved
} tRegister14 ;


typedef struct
{
	uint8_t  ADDR;                    // Register address 0x0F
	uint8_t  SNCP              :3;    // NCP clock source 000: DAC clock 001: Master clock 010: PLL clock 011: OSC clock 100: MCLK clock 101: SCLK clock 110: GPIO
	uint8_t  Reserved          :5;    // Reserved
} tRegister15 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x10
	uint8_t  GDAC              :3;    // GPIO Source for DAC clk 101: SDOUT
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  GDSP              :3;    // GPIO Source for uCDSP clk 101: SDOUT
	uint8_t  Reserved2         :1;    // Reserved
} tRegister16 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x11
	uint8_t  GOSR              :3;    // GPIO Source for OSR clk  101: SDOUT
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  GNCP              :3;    // GPIO Source for NCP clk 101: SDOUT
	uint8_t  Reserved2         :1;    // Reserved
} tRegister17 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x12
	uint8_t  GREF              :5;    // GPIO Source for PLL reference clk: SDOUT
	uint8_t  Reserved          :3;    // Reserved
} tRegister18 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x14
	uint8_t  PPDV              :4;    // PLL P � These bits set the PLL divider P factor. These bits are ignored in clock auto	set mode.
	uint8_t  Reserved          :4;    // Reserved
} tRegister20 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x15
	uint8_t  PJDV              :6;    // PLL J � These bits set the J part of the overall PLL multiplication factor J.D * R. These bits are ignored in clock auto set mode
	uint8_t  Reserved          :2;    // Reserved
} tRegister21 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x16
	uint8_t  PDDV              :6;    // PLL D (MSB) � These bits set the D part of the overall PLL multiplication factor J.D * R. These bits are ignored in clock auto set mode.
	uint8_t  Reserved          :2;    // Reserved
} tRegister22 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x17
	uint8_t  PDDV              :8;    // PLL D (LSB) � These bits set the D part of the overall PLL multiplication factor J.D * R. These bits are ignored in clock auto set mode.
} tRegister23 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x18
	uint8_t  PRDV              :4;    // PLL R � These bits set the R part of the overall PLL multiplication factor J.D * R. These bits are ignored in clock auto set mode
	uint8_t  Reserved          :4;    // Reserved
} tRegister24 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x1B
	uint8_t  DDSP              :7;    // DSP Clock Divider � These bits set the source clock divider value for the DSP clock. These bits are ignored in clock auto set mode.
	uint8_t  Reserved          :1;    // Reserved
} tRegister27 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x1C
	uint8_t  DDSP              :7;    // DAC Clock Divider � These bits set the source clock divider value for the DAC clock. These bits are ignored in clock auto set mode.
	uint8_t  Reserved          :1;    // Reserved
} tRegister28 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x1D
	uint8_t  DNCP              :7;    // NCP Clock Divider � These bits set the source clock divider value for the CP clock. These bits are ignored in clock auto set mode.
	uint8_t  Reserved          :1;    // Reserved
} tRegister29 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x1E
	uint8_t  DOSR              :7;    // OSR Clock Divider � These bits set the source clock divider value for the OSR clock. These bits are ignored in clock auto set mode
	uint8_t  Reserved          :1;    // Reserved
} tRegister30 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x20
	uint8_t  DSCLK             :7;    // Master Mode SCLK Divider � These bits set the MCLK divider value to generate I2S master SCLK clock
	uint8_t  Reserved          :1;    // Reserved
} tRegister32 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x21
	uint8_t  DLRK              :8;    // Master Mode LRCLK Divider � These bits set the I2S master SCLK clock divider value	to generate I2S master LRCLK clock
} tRegister33 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x22
	uint8_t  FSSP             : 3;    // FS Speed Mod 011: 48 kHz 100: 88.2-96 kHz
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  I16E              :1;    // 16x Interpolation � This bit enables or disables the 16x interpolation mode 0: 8x interpolation 1: 16x interpolation
	uint8_t  Reserved2         :3;    // Reserved
} tRegister34 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x25
	uint8_t  IPLK              :1;    // Ignore PLL Lock Detection 0: PLL unlocks raise clock error 1: PLL unlocks are ignored
	uint8_t  DCAS              :1;    // Disable Clock Divider Autoset 0: Enable clock auto set 1: Disable clock auto set
	uint8_t  IDCM              :1;    // Ignore LRCLK/SCLK Missing Detection 0: Regard LRCLK/SCLK missing detection 1: Ignore LRCLK/SCLK missing detection
	uint8_t  IDCH              :1;    // Ignore Clock Halt Detection 0: Regard MCLK halt detection  1: Ignore MCLK halt detection
	uint8_t  IDSK              :1;    // Ignore MCLK Detection 0: Regard MCLK detection 1: Ignore MCLK detection
	uint8_t  IDBK              :1;    // Ignore SCLK Detection 0: Regard SCLK detection 1: Ignore SCLK detection
	uint8_t  IDFS              :1;    // Ignore FS Detection 0: Regard FS detection 1: Ignore FS detection
	uint8_t  Reserved          :1;    // Reserved
} tControl37 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x28
	uint8_t  ALEN             : 2;    // I2S Word Length 00: 16 bits 01: 20 bits 10: 24 bits 11: 32 bits
	uint8_t  Reserved1         :2;    // Reserved
	uint8_t  AFMT              :2;    // I2S Data Format 00: I2S 01: DSP 10: RTJ 11: LTJ
	uint8_t  Reserved2         :2;    // Reserved
} tRegister40 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x29
	uint8_t  AOFS              :8;    // I2S Shift
} tRegister41 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x2A
	uint8_t  AUPR             : 2;    // Right DAC Data Path 00: Zero data (mute) 01: Right channel data 10: Left channel data 11: Reserved (do not set)
	uint8_t  Reserved1         :2;    // Reserved
	uint8_t  AUPL              :2;    // Left DAC Data Path 00: Zero data (mute) 01: Left channel data 10: Right channel data 11: Reserved (do not set)
	uint8_t  Reserved2         :2;    // Reserved
} tRegister42 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x2B
	uint8_t  PSEL              :5;    // DSP Program Selection 00001: Rom Mode 1
	uint8_t  Reserved          :3;    // Reserved
} tRegister43 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x2C
	uint8_t  CMDP              :3;    // Clock Missing Detection Period
	uint8_t  Reserved          :5;    // Reserved
} tRegister44 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x3B
	uint8_t  AMTR             : 3;    // Auto Mute Time for Right Channel
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  AUPL              :3;    // Auto Mute Time for Left Channel
	uint8_t  Reserved2         :1;    // Reserved
} tRegister59 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x3C
	uint8_t  PCTL              :2;    // Digital Volume Control 00: The volume for Left and right channels are independent 01: Right channel volume follows left channel setting
	uint8_t  Reserved          :6;    // Reserved
} tRegister60 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x3D
	uint8_t  VOLL              :8;    // Left Digital Volume � These bits control the left channel digital volume. The	digital volume is 24 dB to �103 dB in �0.5 dB step.
} tRegister61;



typedef struct
{
	uint8_t  ADDR;                    // Register address 0x3E
	uint8_t  VOLR              :8;    // Right Digital Volume � These bits control the right channel digital volume. The digital volume is 24 dB to �103 dB in �0.5 dB step.
} tRegister62 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x3F
	uint8_t  VNUS              :2;    // Digital Volume Normal Ramp Up Step
	uint8_t  VNUF              :2;    // Digital Volume Normal Ramp Up Frequency
	uint8_t  VNDS              :2;    // Digital Volume Normal Ramp Down Step
	uint8_t  VNDF              :2;    // Digital Volume Normal Ramp Down Frequency
} tRegister63 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x40
	uint8_t  Reserved          :4;    // Reserved
	uint8_t  VEDS              :2;    // Digital Volume Emergency Ramp Down Step
	uint8_t  VEDF              :2;    // Digital Volume Emergency Ramp Down Frequency
} tRegister64 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x40
	uint8_t  AMRE              :1;    // Auto Mute Right Channel**NOBUS**
	uint8_t  AMLE              :1;    // Auto Mute Left Channel**NOBUS**
	uint8_t  ACTL              :1;    // Auto Mute Control**NOBUS**
	uint8_t  Reserved          :5;    // Reserved
} tRegister65 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x43
	uint8_t  DRPM              :2;    // Right DAC primary DEM dither gain  00: DEM dither gain = 0.5 01: DEM dither gain = 1.0
	uint8_t  DLPM              :2;    // Left DAC primary DEM dither gain  00: DEM dither gain = 0.5 01: DEM dither gain = 1.0
	uint8_t  DRPA              :2;    // Right DAC primary AC dither gain  00: AC dither gain = 0.125  01: AC dither gain = 0.25
	uint8_t  DLPA              :2;    // Left DAC primary AC dither gain  00: AC dither gain = 0.125 01: AC dither gain = 0.25
} tRegister67 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x44
	uint8_t  DLPD              :3;    // Left DAC primary DC dither
	uint8_t  Reserved          :5;    // Reserved
} tRegister68 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x45
	uint8_t  DLPD              :8;    // Left DAC primary DC dither
} tRegister69 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x46
	uint8_t  DRPD              :8;    // Right DAC primary DC dither
} tRegister70 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x47
	uint8_t  DRPD              :8;    // Right DAC primary DC dither
} tRegister71 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x48
	uint8_t  RSM               :2;    // Right DAC secondary DEM dither gain
	uint8_t  DLSM              :2;    // Left DAC secondary DEM dither gain
	uint8_t  DRSA              :2;    // Right DAC secondary AC dither gain
	uint8_t  DLSA              :2;    // Left DAC secondary AC dither gain
} tRegister72 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x49
	uint8_t  DLSD              :8;    // Left DAC secondary DC dither
} tRegister73 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x4A
	uint8_t  DLSD              :8;    // Left DAC secondary DC dither
} tRegister74 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x4B
	uint8_t  DRSD              :8;    // Right DAC secondary DC dither
} tRegister75 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x4C
	uint8_t  DRSD              :8;    // Right DAC secondary DC dither
} tRegister76 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x4E
	uint8_t  OLOF              :8;    // Left OFSCAL offset
} tRegister78 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x4F
	uint8_t  OLOF              :8;    // Right OFSCAL offset
} tRegister79 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x55
	uint8_t  G2SL              :5;    // SDOUT Output Selection
	uint8_t  Reserved          :3;    // Reserved
} tRegister85 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x56
	uint8_t  Reserved1         :4;    // Reserved
	uint8_t  MUTE              :1;    // This bit controls the MUTE output when the selection at P0-R84 is set to 0010 (register output). 0: Output low 1: Output high
	uint8_t  GOUT2             :1;    // GPIO Output Control  0: Output low 1: Output high
	uint8_t  Reserved2         :2;    // Reserved
} tRegister86 ;


typedef struct
{
	uint8_t  ADDR;                    // Register address 0x57
	uint8_t  Reserved1         :4;    // Reserved
	uint8_t  MUTE              :1;    // This bit controls the polarity of MUTE output. When set to 1, the output will be inverted	for any signal being selected. 0: Non-inverted 1: Inverted
	uint8_t  GINV2             :1;    // GPIO Output Inversion 0: Non-inverted 1: Inverted
	uint8_t  Reserved2         :2;    // Reserved
} tRegister87 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x58
	uint8_t  DIEI              :8;    // Die ID, Device ID = 0x84
} tRegister88 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x5B
	uint8_t  DTSR              :4;    // Detected MCLK Ratio 1000: MCLK = 512 FS 0110: MCLK = 256 FS
	uint8_t  DTFS              :3;    // Detected FS  011: 32-48 kHz 100: 88.2-96 kHz
	uint8_t  Reserved          :1;    // Reserved
} tRegister91 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x5C
	uint8_t  DTBR              :1;    // Detected SCLK Ratio (MSB)
	uint8_t  Reserved          :7;    // Reserved
} tRegister92 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x5D
	uint8_t  DTBR              :8;    // Detected SCLK Ratio (LSB)
} tRegister93 ;


typedef struct
{
	uint8_t  ADDR;                    // Register address 0x5E
	uint8_t  CDST0             :1;    // This bit indicated whether the audio sampling rate is valid or not.  0: Sampling rate is valid 1: Sampling rate is invalid
	uint8_t  CDST1             :1;    // This bit indicates whether the SCLK is valid or not. 0: SCLK is valid 1: SCLK is invalid
	uint8_t  CDST2             :1;    // This bit indicates whether the MCLK is valid or not. 0: MCLK is valid 1: MCLK is invalid
	uint8_t  CDST3             :1;    // This bit indicates whether the combination of current sampling rate and MCLK ratio is	valid for clock auto set. 0: The combination of FS/MCLK ratio is valid 1: Error (clock auto set is not possible)
	uint8_t  CDST4             :1;    // This bit indicates whether the both LRCLK and SCLK are missing (tied low) or not. 0: LRCLK and/or SCLK is present 1: LRCLK and SCLK are missing
	uint8_t  CDST5             :1;    // This bit indicates whether the PLL is locked or not. The PLL will be reported as unlocked when it is disabled. 0: PLL is locked 1: PLL is unlocked
	uint8_t  CDST6             :1;    // Clock Detector Status � This bit indicates whether the MCLK clock is present or not 0: MCLK is present 1: MCLK is missing (halted)
	uint8_t  Reserved          :1;    // Reserved
} tControl94 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x5F
	uint8_t  CERF              :1;    // Clock Error  0: Clock error has not occurred 1: Clock error has occurred.
	uint8_t  CSRF              :1;    // Clock Resync Request  0: Not resynchronizing 1: Clock resynchronization is in progress
	uint8_t  CKMF              :1;    // Clock Missing  0: LRCLK and/or SCLK is present 1: LRCLK and SCLK are missing
	uint8_t  Reserved1         :1;    // Reserved
	uint8_t  LTSH              :1;    // Latched Clock Halt 0: MCLK halt has not occurred 1: MCLK halt has occurred since last read
	uint8_t  Reserved2         :3;    // Reserved
} tControl95 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x6C
	uint8_t  AMRM              :1;    // Right Analog Mute Monitor 0: Mute 1: Unmute
	uint8_t  AMLM              :1;    // Left Analog Mute Monitor 0: Mute 1: Unmute
	uint8_t  Reserved          :6;    // Reserved
} tControl108 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x77
	uint8_t  Reserved1         :4;    // Reserved
	uint8_t  MUTE              :1;    // This bit indicates the logic level at MUTE pin  0: Low 1: High
	uint8_t  GPIN2             :1;    // GPIO Input States � This bit indicates the logic level at SDOUT pin. 0: Low 1: High
	uint8_t  Reserved2         :2;    // Reserved
} tControl119 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x78
	uint8_t  AMFR              :1;    // Auto Mute Flag for Right Channel 0: Not auto muted 1: Auto muted
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  AMFL              :1;    // Auto Mute Flag for Left Channel  0: Not auto muted 1: Auto muted
	uint8_t  Reserved2         :3;    // Reserved
} tControl120 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x01
	uint8_t  OSEL              :8;    // Output Amplitude Type  0: VREF mode 1: VCOM mode
} tControlP1R1 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x02
	uint8_t  RAGN              :1;    // Analog Gain Control for Right Channel
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  LAGN              :1;    // Analog Gain Control for Left Channel
	uint8_t  Reserved2         :3;    // Reserved
} tControlP1R2 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x06
	uint8_t  AMCT              :1;    // Analog Mute Control  0: Disabled 1: Enabled
	uint8_t  Reserved          :7;    // Reserved
} tControlP1R6 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x07
	uint8_t  AGBR              :1;    // Analog +10% Gain for Right Channel
	uint8_t  Reserved1         :3;    // Reserved
	uint8_t  AGBL              :1;    // Analog +10% Gain for Left Channel
	uint8_t  Reserved2         :3;    // Reserved
} tControlP1R7 ;

typedef struct
{
	uint8_t  ADDR;                    // Register address 0x09
	uint8_t  VCPD              :1;    // Power down control for VCOM
	uint8_t  DEME              :1;    // VCOM Pin as De-emphasis Control
	uint8_t  Reserved          :6;    // Reserved
} tControlP1R9 ;



typedef struct
{
	tRegister2 Register2;
	tRegister3 Register3;
	tRegister4 Register4;
	tRegister6 Register6;
	tRegister7 Register7;
	tRegister8 Register8;
	tRegister9 Register9;
	tRegister12 Register12;
	tRegister13 Register13;
	tRegister14 Register14;
	tRegister15 Register15;
	tRegister16 Register16;
	tRegister17 Register17;
	tRegister18 Register18;
	tRegister20 Register20;
	tRegister21 Register21;
	tRegister22 Register22;
	tRegister23 Register23;
	tRegister24 Register24;
	tRegister27 Register27;
	tRegister28 Register28;
	tRegister29 Register29;
	tRegister30 Register30;
	tRegister32 Register32;
	tRegister33 Register33;
	tRegister34 Register34;
	tControl37 Control37;
	tRegister40 Register40;
	tRegister41 Register41;
	tRegister42 Register42;
	tRegister43 Register43;
	tRegister44 Register44;
	tRegister59 Register59;
	tRegister60 Register60;
	tRegister61 Register61;
	tRegister62 Register62_TAS3251;
	tRegister63 Register63;
	tRegister64 Register64;
	tRegister65 Register65;
	tRegister67 Register67;
	tRegister68 Register68;
	tRegister69 Register69;
	tRegister70 Register70;
	tRegister71 Register71;
	tRegister72 Register72;
	tRegister73 Register73;
	tRegister74 Register74;
	tRegister75 Register75;
	tRegister76 Register76;
	tRegister78 Register78;
	tRegister79 Register79;
	tRegister85 Register85;
	tRegister86 Register86;
	tRegister87 Register87;
	tRegister88 Register88;
	tRegister91 Register91;
	tRegister92 Register92;
	tRegister93 Register93;
	tControl94 tControl94;
	tControl95 tControl95;
	tControl108 Control108;
	tControl119 Control119;
	tControl120 Control120;
	tControlP1R1 ControlP1R1;
	tControlP1R2 ControlP1R2;
	tControlP1R6 ControlP1R6;
	tControlP1R7 ControlP1R7;
	tControlP1R9 ControlP1R9;
} sDAC_TAS3251;
























#endif
