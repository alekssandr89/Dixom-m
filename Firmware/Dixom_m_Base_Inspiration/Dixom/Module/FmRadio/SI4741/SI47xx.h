#ifndef __SI47XX_H__
#define __SI47XX_H__
#include "stdint.h"

void      Init_SI47xx(void);

void      SI_4741_Set_Spacing();

void      SI47xx_GET_RADIO_FREQ(void);
void      SI47xx_RADIO_AUTO_SEARCH(uint8_t *Recive_USB);
void      SI47xx_RADIO_MANUAL_SEARCH(uint8_t *Recive_USB);
//void      SET_SI47xx_FREQ( uint8_t *Recive_USB );
void      GET_SI47xx_InitInfo(void);
//void      SI47xx_SET_PRESET(uint8_t *Recive_USB);

void Test1();
void Test2();
void Test3();
void Test4();



void      SI47xx_setFreq();
void      SI47xx_GetFreq(void);
void      SI47xx_AutoSearch(uint8_t seek);
void      SI47xx_ManualSearch(uint8_t seek);

void      SI47xx_GetAgc(void);



uint8_t   GetStatus(uint8_t GetInt);
void      GetRevision(void);
void      GetTuneStatus(void);
void      GetRsqStatus(uint8_t Get);
void      GetRdsStatus(void);

typedef enum
{
	CTS,
	ERR,
	RSQINT,
	RDSINT,
	STCINT,
}tGET_INT_STATUS;



typedef struct
{
	uint8_t  STCINT        :1;    // 0 Seek/Tune Complete Interrupt.	0 = Tune complete has not been triggered.	1 = Tune complete has been triggered.
	uint8_t  Reserved0     :1;    // 1 Reserved
	uint8_t  RDSINT        :1;    // 2 RDS Interrupt.	0 = RDS interrupt has not been triggered.	1 = RDS interrupt has been triggered.
	uint8_t  RSQINT        :1;    // 3 Master Clock Select
	uint8_t  Reserved1     :1;    // 4 Reserved
	uint8_t  Reserved2     :1;    // 5 Reserved
	uint8_t  ERR           :1;    // 6 Error.	0 = No error	1 = Error
	uint8_t  CTS           :1;    // 7 Clear to Send.	0 = Wait before sending next command.	1 = Clear to send next command.
} tSI_4741_STATUS;




typedef struct
{
	uint8_t  PN             ;    // Final 2 digits of Part Number (HEX)
	uint8_t  FWMAJOR        ;    // Firmware Major Revision (ASCII)
	uint8_t  FWMINOR        ;    // Firmware Minor Revision (ASCII)
	uint8_t  PATCHH         ;    // Patch ID High Byte (HEX)
	uint8_t  PATCHL         ;    // Patch ID Low Byte (HEX)
	uint8_t  CMPMAJOR       ;    // Component Major Revision (ASCII)
	uint8_t  CMPMINOR       ;    // Component Minor Revision (ASCII)
	uint8_t  CHIPREV        ;    // Chip Revision (ASCII)
	uint8_t  LIBRARYID      ;    // Library Revision (HEX).
} tSiRevision;

typedef struct
{
	uint8_t  VALID         :1;    // Valid Channel.	Set if the channel is currently valid as determined by the seek/tune properties (0x1403, 0x1404, 0x1108) and would have been found during a Seek.
	uint8_t  AFCRL         :1;    // AFC Rail Indicator.	Set if the AFC rails.
	uint8_t  Reserved      :5;    // Reserved
	uint8_t  BLTF          :1;    // Band Limit.	Reports if a seek hit the band limit (WRAP = 0 in FM_START_SEEK) or	wrapped to the original frequency (WRAP = 1).
} tSiTuneResp1;

typedef struct
{
	tSiTuneResp1  RESP1     ;    // Final 2 digits of Part Number (HEX)
	uint8_t  READFREQH      ;    // Read Frequency High Byte.	This byte in combination with READFREQL returns frequency being tuned	(10 kHz).
	uint8_t  READFREQL      ;    // Read Frequency Low Byte.	This byte in combination with READFREQH returns frequency being tuned	(10 kHz).
	uint8_t  RSSI           ;    // Received Signal Strength Indicator.	This byte contains the receive signal strength when tune is complete (dB�V).
	uint8_t  SNR            ;    // SNR.	This byte contains the SNR metric when tune is complete (dB).
	uint8_t  MULT           ;    // Multipath.	This byte contains the multipath metric when tune is complete. Multipath indicator is available only for Si474x, Si4706-C30 and later and	Si4704/05/30/31/34/35/84/85 -D50 and later, and Si4732.
} tSiTuneStatus;



typedef struct
{
	uint8_t  RSSILINT      :1;    // RSSI Detect Low.	0 = RSSI has not exceeded above RSSI high threshold.	1 = RSSI has exceeded above RSSI high threshold.
	uint8_t  RSSIHINT      :1;    // RSSI Detect High.	0 = RSSI has not exceeded above RSSI high threshold.	1 = RSSI has exceeded above RSSI high threshold.
	uint8_t  SNRLINT       :1;    // SNR Detect Low.	0 = Received SNR has not exceeded above SNR high threshold.	1 = Received SNR has exceeded above SNR high threshold.
	uint8_t  SNRHINT       :1;    // SNR Detect High.	0 = Received SNR has not exceeded above SNR high threshold.	1 = Received SNR has exceeded above SNR high threshold.
	uint8_t  MULTLINT      :1;    // Multipath Detect Low 0 = Detected multipath value has not exceeded above the Multipath high threshold. 1 = Detected multipath value has exceeded above the Multipath high threshold.
	uint8_t  MULTHINT      :1;    // Multipath Detect High 0 = Detected multipath value has not exceeded above the Multipath high threshold. 1 = Detected multipath value has exceeded above the Multipath high threshold.
	uint8_t  Reserved      :1;    // SNR.	This byte contains the SNR metric when tune is complete (dB).
	uint8_t  BLENDINT      :1;    // Blend Detect Interrupt.	0 = Blend is within the Blend threshold settings.	1 = Blend goes above or below the Blend threshold settings.
} tSiRsqRESP1;


typedef struct
{
	uint8_t  VALID         :1;    // Valid Channel.	Set if the channel is currently valid and would have been found during a Seek.
	uint8_t  AFCRL         :1;    // AFC Rail Indicator.	Set if the AFC rails.
	uint8_t  Reserved0     :1;    // Reserved
	uint8_t  SMUTE         :1;    // Soft Mute Indicator.	Indicates soft mute is engaged.
	uint8_t  Reserved1     :4;    // Reserved
} tSiRsqRESP2;


typedef struct
{
	uint8_t  STBLEND       :7;    // Stereo Blend Indicator.	Indicates amount of stereo blend in% (100 = full stereo, 0 = full mono)
	uint8_t  PILOT         :1;    // Pilot Indicator.	Indicates stereo pilot presence.
} tSiRsqRESP3;


typedef struct
{
	tSiRsqRESP1  RESP1        ;    // RESP1
	tSiRsqRESP2  RESP2        ;    // RESP2
	tSiRsqRESP3  RESP3        ;    // RESP3
	uint8_t      RSSI         ;    // Received Signal Strength Indicator.	Contains the current receive signal strength (0�127 dB�V).
	uint8_t      SNR          ;    // SNR.	Contains the current SNR metric (0�127 dB).
	uint8_t      MULT         ;    // Multipath Contains the current multipath metric. (0 = no multipath; 100 = full multipath)
	uint8_t      FREQOFF      ;    // Frequency Offset.	Signed frequency offset (kHz).
} tSiFmRsqStatus;


typedef struct
{
	uint8_t  RDSRECV         :1;    // RDS Received.	1 = FIFO filled to minimum number of groups set by RDSFIFOCNT.
	uint8_t  RDSSYNCLOST     :1;    // RDS Sync Lost.	1 = Lost RDS synchronization.
	uint8_t  RDSSYNCFOUND    :1;    // RDS Sync Found.	1 = Found RDS synchronization.
	uint8_t  Reserved0       :1;    // Reserved
	uint8_t  RDSNEWBLOCKA    :1;    // RDS New Block A.	1 = Valid Block A data has been received.
	uint8_t  RDSNEWBLOCKB    :1;    // RDS New Block B.	1 = Valid Block B data has been received.
	uint8_t  Reserved1       :1;    // Reserved
	uint8_t  Reserved2       :1;    // Reserved
} tSiRdsRESP1;

typedef struct
{
	uint8_t  RDSSYNC         :1;    // RDS Sync.	1 = RDS currently synchronized.
	uint8_t  Reserved0       :1;    // Reserved
	uint8_t  GRPLOST         :1;    // Group Lost.	1 = One or more RDS groups discarded due to FIFO overrun.
	uint8_t  Reserved1       :1;    // Reserved
	uint8_t  Reserved2       :1;    // Reserved
	uint8_t  Reserved3       :1;    // Reserved
	uint8_t  Reserved4       :1;    // Reserved
	uint8_t  Reserved5       :1;    // Reserved
} tSiRdsRESP2;

typedef struct
{
	uint8_t  BLED           :2;    // RDS Block A Corrected Errors.         	0 = No errors.
	uint8_t  BLEC           :2;    // RDS Block B Corrected Errors.            	1 = 1�2 bit errors detected and corrected.
	uint8_t  BLEB           :2;    // RDS Block C Corrected Errors.             2 = 3�5 bit errors detected and corrected.
	uint8_t  BLEA           :2;    // RDS Block D Corrected Errors.             3 = Uncorrectable.

} tSiRdsRESP12;


typedef struct
{
	tSiRdsRESP1  RESP1        ;    // RESP1
	tSiRdsRESP2  RESP2        ;    // RESP2
	uint8_t      RDSFIFOUSED  ;    // RDS FIFO Used.	Number of groups remaining in the RDS FIFO (0 if empty). If non-zero, BLOCKA-BLOCKD contain the oldest FIFO entry
	                               // and RDSFIFOUSED decrements by one on the next call to RDS_FIFO_STATUS (assuming no RDS	data received in the interim).
	uint8_t      RESP4_BLOCKA1      ;    // RDS Block A group data from oldest FIFO entry if STATUSONLY is 0. Last valid	Block A data if STATUSONLY is 1
	uint8_t      RESP5_BLOCKA2      ;
	uint8_t      RESP6_BLOCKB1      ;    // RDS Block B group data from oldest FIFO entry if STATUSONLY is 0. Last valid	Block B data if STATUSONLY is 1
	uint8_t      RESP7_BLOCKB2      ;
	uint8_t      RESP8_BLOCKC1      ;    // RDS Block C group data from oldest FIFO entry
	uint8_t      RESP9_BLOCKC2      ;
	uint8_t      RESP10_BLOCKD1      ;    // RDS Block D group data from oldest FIFO entry
	uint8_t      RESP11_BLOCKD2      ;
	tSiRdsRESP12 RESP12;
} tSiRdsStatus;




typedef struct
{
	tSI_4741_STATUS STATUS;
	tSiRevision     REV;
	tSiTuneStatus   FM_TUNE_STATUS;
	tSiFmRsqStatus  FM_RSQ_STATUS;
	tSiRdsStatus    FM_RDS_STATUS;
} sSiRegister;


typedef struct
{
	sSiRegister       Register;
	uint16_t          Freq;

} sSiRadio;






//////////////////////// --Command 0x14. GET_INT_STATUS-- /////////////////

typedef struct
{
	uint8_t  STCINT        :1;    // Seek/Tune Complete Interrupt.	0 = Tune complete has not been triggered.	1 = Tune complete has been triggered.
	uint8_t  Reserved0     :1;    // Reserved
	uint8_t  RDSINT        :1;    // RDS Interrupt.	0 = RDS interrupt has not been triggered.	1 = RDS interrupt has been triggered.
	uint8_t  RSQINT        :1;    // Master Clock Select
	uint8_t  Reserved1     :1;    // Reserved
	uint8_t  Reserved2     :1;    // Reserved
	uint8_t  ERR           :1;    // Error.	0 = No error	1 = Error
	uint8_t  CTS           :1;    // Clear to Send.	0 = Wait before sending next command.	1 = Clear to send next command.
} sSI_4741_STATUS;

//////////////////////// --Command 0x01. POWER_UP-- /////////////////

typedef struct
{
	uint8_t     CMD;
	uint8_t     FUNC :4;
    uint8_t     XOSCEN: 1;
    uint8_t     PATCH: 1;
    uint8_t     GPO2OEN: 1;
    uint8_t     CTSIEN: 1;
    uint8_t     OPMODE;
	struct
	{
    	sSI_4741_STATUS  STATUS;           //Status Response for the AM/SW/LW Receiver
	 	uint8_t          PN;               //Final 2 digits of part number (HEX)
	 	uint8_t          FWMAJOR;          //Firmware Major Revision (ASCII)
	 	uint8_t          FWMINOR;          //Firmware Minor Revision (ASCII)
	 	uint8_t          Reserved1;
	 	uint8_t          Reserved2;
	 	uint8_t          CHIPREV;          //Chip Revision (ASCII)
	 	uint8_t          LIBRARYID;	 	//Library Revision (HEX).
	 }Response;
} sSI_4741_POWER_UP;

//////////////////////// --Command 0x10. GET_REV-- /////////////////

typedef struct
{
	uint8_t CMD;
	struct
	{
		sSI_4741_STATUS  STATUS;         //Status Response for the AM/SW/LW Receiver
		uint8_t          PN;            // Final 2 digits of Part Number (HEX)
		uint8_t          FWMAJOR;       // Firmware Major Revision (ASCII)
		uint8_t          FWMINOR;       // Firmware Minor Revision (ASCII)
		uint8_t          PATCHH;        // Patch ID High Byte (HEX)
		uint8_t          PATCHL;        // Patch ID Low Byte (HEX)
		uint8_t          CMPMAJOR;      // Component Major Revision (ASCII)
		uint8_t          CMPMINOR;      // Component Minor Revision (ASCII)
		uint8_t          CHIPREV;       // Chip Revision (ASCII)
	}Response;
} sSI_4741_GET_REV;

//////////////////////// --Command 0x12. SET_PROPERTY-- /////////////////

typedef struct
{
	uint8_t CMD;
	uint8_t ARG1;
	struct
	{
		uint16_t  PROP_ADDR;            // Property
		uint16_t  PROP_VALUE;           // Property Value
	}Property;
} sSI_4741_SET_PROPERTY;

//////////////////////// --Command 0x13. GET_PROPERTY-- /////////////////

typedef struct
{
	uint8_t  CMD;
	uint8_t  ARG1;
	uint16_t PROP_ADDR;                  // Property addres
	struct
	{
		sSI_4741_STATUS  STATUS;         // Status Response for the AM/SW/LW Receiver
		uint8_t          RESP1;          // Always returns 0.
		uint16_t         PROP_VALUE;     // Property Value (RESP2, RESP3)
	}Response;
} sSI_4741_GET_PROPERTY;

//////////////////////// --Command 0x20. FM_TUNE_FREQ-- /////////////////

typedef struct
{
	uint8_t CMD;
	struct
	{
		uint8_t  FAST:1;
	 	uint8_t  FREEZE:1;
	 	uint8_t  Reserved1:6;
	}ARG1;
	uint8_t FREQH;
	uint8_t FREQL;
	uint8_t ANTCAP;

} sSI_4741_TUNE_FREQ;

//////////////////////// --Command 0x21. FM_SEEK_START-- /////////////////

typedef struct
{
	uint8_t CMD;
	struct
	{
		uint8_t  Reserved1:1;
	 	uint8_t  Reserved2:1;
	 	uint8_t  WRAP:1;
	 	uint8_t  SEEKUP:1;
	 	uint8_t  Reserved3:4;
	}ARG1;
} sSI_4741_SEEK_START;

//////////////////////// --Command 0x22. FM_TUNE_STATUS-- /////////////////


typedef struct
{
	uint8_t CMD;
	struct
	{
		uint8_t  INTACK:1;
	 	uint8_t  CANCEL:1;
	 	uint8_t  Reserved1:6;
	}ARG1;
	struct
	{
		sSI_4741_STATUS STATUS;
		struct
		{
			uint8_t  VALID:1;
		 	uint8_t  AFCRL:1;
		 	uint8_t  Reserved1:5;
		 	uint8_t  BLTF:1;
		}RESP1;
	 	uint8_t  READFREQH;
	 	uint8_t  READFREQL;
	 	uint8_t  RSSI;
	 	uint8_t  SNR;
	 	uint8_t  MULT;
	}Response;
} sSI_4741_TUNE_STATUS;

//////////////////////// --Command 0x23. FM_RSQ_STATUS-- /////////////////

typedef struct
{
	uint8_t CMD;
	struct
	{
		uint8_t  INTACK:1;
	 	uint8_t  Reserved1:7;
	}ARG1;
	struct
	{
		sSI_4741_STATUS STATUS;
		struct
		{
			uint8_t  RSSIILINT:1;
		 	uint8_t  RSSIHINT:1;
		 	uint8_t  SNRLINT:1;
		 	uint8_t  SNRHINT:1;
			uint8_t  MULTLINT:1;
		 	uint8_t  MULTHINT:1;
		 	uint8_t  Reserved1:1;
		 	uint8_t  BLENDINT:1;
		}RESP1;
		struct
		{
			uint8_t  VALID:1;
		 	uint8_t  AFCRL:1;
		 	uint8_t  Reserved1:1;
		 	uint8_t  SMUTE:1;
		 	uint8_t  Reserved2:1;
		}RESP2;
		struct
		{
			uint8_t  STBLEND:7;
		 	uint8_t  PILOT:1;
		}RESP3;
	 	uint8_t  RSSI;
	 	uint8_t  SNR;
	 	uint8_t  MULT;
	 	uint8_t  FREQOFF;
	}Response;
} sSI_4741_RSQ_STATUS;

//////////////////////// Property 0x0001. GPO_IEN /////////////////
typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t  STCIEN:1;
	 	uint8_t  Reserved1:1;
	 	uint8_t  RDSIEN:1;
	 	uint8_t  RSQIEN:1;
	 	uint8_t  Reserved2:2;
	 	uint8_t  ERRIEN:1;
	 	uint8_t  CTSIEN:1;
	 	uint8_t  STCREP:1;
	 	uint8_t  Reserved3:1;
	 	uint8_t  RDSREP:1;
	 	uint8_t  RSQREP:1;
	 	uint8_t  Reserved4:4;
	}ARG1;
} sSI_4741_GPO_IEN;

//////////////////////// Property 0x0102. DIGITAL_OUTPUT_FORMAT /////////////////
typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t  OSIZE:2;
	 	uint8_t  OMONO:1;
	 	uint8_t  OMODE:4;
	 	uint8_t  OFALL:1;
	 	uint8_t  Reserved1:8;
	}ARG1;
} sSI_4741_DIGITAL_OUTPUT_FORMAT;

//////////////////////// Property 0x0104. DIGITAL_OUTPUT_SAMPLE_RATE /////////////////
typedef struct
{
	uint16_t Property;
	uint16_t DOSR;
} sSI_4741_DIGITAL_OUTPUT_SAMPLE_RATE;

//////////////////////// Property 0x0201. REFCLK_FREQ /////////////////

typedef struct
{
	uint16_t Property;
	uint16_t REFCLKF;
} sSI_4741_REFCLK_FREQ;

//////////////////////// Property 0x0202. REFCLK_PRESCALE /////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint16_t REFCLKP:12;
		uint8_t  RCLK_SEL:1;
		uint8_t  Reserved1:3;
	}ARG1;

} sSI_4741_REFCLK_PRESCALE;

//////////////////////// Property 0x1100. FM_DEEMPHASIS /////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   DEEMPH:2;
		uint16_t  Reserved1:14;
	}ARG1;
} sSI_4741_FM_DEEMPHASIS;

//////////////////////// Property 0x1102. FM_CHANNEL_FILTER/////////////////

typedef struct
{
	uint16_t Property;
    uint16_t FMCHFILT;
} sSI_4741_FM_CHANNEL_FILTER;

//////////////////////// Property 0x1105. FM_BLEND_STEREO_THRESHOLD/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   STTHRESH:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_BLEND_STEREO_THRESHOLD;

//////////////////////// Property 0x1106. FM_BLEND_MONO_THRESHOLD/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint16_t   REFCLKP:12;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_BLEND_MONO_THRESHOLD;


////////////////////////Property 0x1200. FM_RSQ_INT_SOURCE/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t  RSSILIEN:1;
		uint8_t  RSSIHIEN:1;
		uint8_t  SNRLIEN:1;
		uint8_t  SNRHIEN:1;
		uint8_t  MULTLIEN:1;
		uint8_t  MULTHIEN:1;
		uint8_t  Reserved1:1;
		uint8_t  BLENDIEN:1;
		uint8_t  Reserved2:8;
	}ARG1;
} sSI_4741_FM_RSQ_INT_SOURCE;

//////////////////////// Property 0x4000. RX_VOLUME/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   VOL:6;
		uint16_t  Reserved1:10;
	}ARG1;
} sSI_4741_RX_VOLUME;

//////////////////////// Property 0x1800. FM_BLEND_RSSI_STEREO_THRESHOLD/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   STRTHRESH:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_BLEND_RSSI_STEREO_THRESHOLD;

//////////////////////// Property 0x1801. FM_BLEND_RSSI_MONO_THRESHOLD/////////////////

typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   MONOTHRESH:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_BLEND_RSSI_MONO_THRESHOLD;


//////////////////////// Property 0x1802. FM_BLEND_RSSI_ATTACK_RATE/////////////////


typedef struct
{
	uint16_t Property;
    uint16_t ATTACK;
} sSI_4741_FM_BLEND_RSSI_ATTACK_RATE;

//////////////////////// Property 0x1803. FM_BLEND_RSSI_RELEASE_RATE/////////////////


typedef struct
{
	uint16_t Property;
    uint16_t RELEASE;
} sSI_4741_FM_BLEND_RSSI_RELEASE_RATE;

//////////////////////// Property 0x1804. FM_BLEND_SNR_STEREO_THRESHOLD/////////////////


typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   STRTHRESH:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_BLEND_SNR_STEREO_THRESHOLD;


//////////////////////// Property 0x1400. FM_SEEK_BAND_BOTTOM/////////////////


typedef struct
{
	uint16_t Property;
	uint16_t FMSKFREQL;
} sSI_4741_FM_SEEK_BAND_BOTTOM;

//////////////////////// Property 0x1401. FM_SEEK_BAND_TOP/////////////////


typedef struct
{
	uint16_t Property;
	uint16_t FMSKFREQH;
} sSI_4741_FM_SEEK_BAND_TOP;


//////////////////////// Property 0x1402. FM_SEEK_FREQ_SPACING/////////////////


typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   SKSPACE:5;
		uint16_t  Reserved1:11;
	}ARG1;
} sSI_4741_FM_SEEK_FREQ_SPACING;

//////////////////////// Property 0x1403. FM_SEEK_TUNE_SNR_THRESHOLD/////////////////


typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   SKSNR:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_SEEK_TUNE_SNR_THRESHOLD;

//////////////////////// Property 0x1404. FM_SEEK_TUNE_RSSI_THRESHOLD/////////////////


typedef struct
{
	uint16_t Property;
	struct
	{
		uint8_t   SKRSSI:7;
		uint16_t  Reserved1:9;
	}ARG1;
} sSI_4741_FM_SEEK_TUNE_RSSI_THRESHOLD;


typedef struct
{
	sSI_4741_POWER_UP    POWER_UP;
	sSI_4741_GET_REV     GET_REV;
	sSI_4741_STATUS      STATUS;
	sSI_4741_RSQ_STATUS  RSQ_STATUS;
	sSI_4741_TUNE_FREQ   TUNE_FREQ;
	sSI_4741_SEEK_START  SEEK_START;
	sSI_4741_TUNE_STATUS TUNE_STATUS;
	sSI_4741_SET_PROPERTY SET_PROPERTY;
	sSI_4741_GET_PROPERTY GET_PROPERTY;
	sSI_4741_DIGITAL_OUTPUT_FORMAT DIGITAL_OUTPUT_FORMAT;
	sSI_4741_DIGITAL_OUTPUT_SAMPLE_RATE DIGITAL_OUTPUT_SAMPLE_RATE;
	sSI_4741_REFCLK_FREQ REFCLK_FREQ;
	sSI_4741_REFCLK_PRESCALE  REFCLK_PRESCALE;
	sSI_4741_FM_DEEMPHASIS    DEEMPHASIS;
	sSI_4741_FM_CHANNEL_FILTER CHANNEL_FILTER;
	sSI_4741_FM_BLEND_STEREO_THRESHOLD BLEND_STEREO_THRESHOLD;
	sSI_4741_FM_BLEND_MONO_THRESHOLD BLEND_MONO_THRESHOLD;
	sSI_4741_FM_CHANNEL_FILTER FM_CHANNEL_FILTER;
	sSI_4741_FM_RSQ_INT_SOURCE FM_RSQ_INT_SOURCE;
	sSI_4741_GPO_IEN GPO_IEN;
	sSI_4741_RX_VOLUME RX_VOLUME;
	sSI_4741_FM_BLEND_RSSI_STEREO_THRESHOLD FM_BLEND_RSSI_STEREO_THRESHOLD;
	sSI_4741_FM_BLEND_RSSI_MONO_THRESHOLD FM_BLEND_RSSI_MONO_THRESHOLD;
	sSI_4741_FM_BLEND_RSSI_ATTACK_RATE FM_BLEND_RSSI_ATTACK_RATE;
	sSI_4741_FM_BLEND_RSSI_RELEASE_RATE FM_BLEND_RSSI_RELEASE_RATE;
	sSI_4741_FM_BLEND_SNR_STEREO_THRESHOLD FM_BLEND_SNR_STEREO_THRESHOLD;

	sSI_4741_FM_SEEK_BAND_BOTTOM  FM_SEEK_BAND_BOTTOM;
	sSI_4741_FM_SEEK_BAND_TOP FM_SEEK_BAND_TOP;
	sSI_4741_FM_SEEK_FREQ_SPACING FM_SEEK_FREQ_SPACING;
	sSI_4741_FM_SEEK_TUNE_SNR_THRESHOLD FM_SEEK_TUNE_SNR_THRESHOLD;
	sSI_4741_FM_SEEK_TUNE_RSSI_THRESHOLD FM_SEEK_TUNE_RSSI_THRESHOLD;

} sFM_Radio_SI4735;










#define FIELD_RDS_STATUS_RESP12_BLOCK_A        0b11000000
#define FIELD_RDS_STATUS_RESP12_BLOCK_B       0b00110000
#define FIELD_RDS_STATUS_RESP12_BLOCK_C       0b00001100
#define FIELD_RDS_STATUS_RESP12_BLOCK_D        0b00000011
#define RDS_STATUS_RESP12_BLOCK_D_NO_ERRORS      0U<<0
#define RDS_STATUS_RESP12_BLOCK_C_NO_ERRORS     0U<<2
#define RDS_STATUS_RESP12_BLOCK_B_NO_ERRORS     0U<<4

enum {
   RDS_THRESHOLD=3,      //Threshold for larger variables
   RDS_BOOL_THRESHOLD=7  //Threshold for boolean variables
};

//typedef signed char ternary;
























#endif
