#ifndef __SI47XXREG_H__
#define __SI47XXREG_H__
#include "stdio.h"




typedef struct
{
	uint8_t   STCINT    :1; //  0    Seek/Tune Complete Interrupt.	0 = Tune complete has not been triggered.	1 = Tune complete has been triggered.
 	uint8_t   Reserved1 :1;  // 2-1
 	uint8_t   RDSINT    :1;  // 2-1  Radio Data System (RDS) Interrupt (Si4705/21/31/32/35/37/39/85 Only). 	0 = Radio data system interrupt has not been triggered. 	1 = Radio data system interrupt has been triggered
 	uint8_t   RSQINT    :1;  // 3    Received Signal Quality Interrupt. 	0 = Received Signal Quality measurement has not been triggere 	1 = Received Signal Quality measurement has been triggered
 	uint8_t   Reserved2 :2;  // 5-4
 	uint8_t   ERR       :1;  // 6    Error. 	0 = No err 	1 = Error
 	uint8_t   CTS       :1;  // 7    Clear to Send.	0 = Wait before sending next comm	1 = Clear to send next command
} sSTAT4735;












typedef struct
{
	 union {
		 struct
		 {
		 	uint8_t   FUNC         :4;
		 	uint8_t   XOSCEN       :1;
		 	uint8_t   PATCH        :1;
		 	uint8_t   GPO2OEN      :1;
		 	uint8_t   CTSIEN       :1;
		 }Bit;
		 uint8_t Val;
	 }POWER_UP;
} tRadio;

typedef struct{
	struct{
		uint8_t   POWER_UP        :1; //0x01
		uint8_t   GET_REV         :5; //0x10
		uint8_t   POWER_DOWN      :5; //0x11
		uint8_t   SET_PROPERTY    :5; //0x12
		uint8_t   GET_PROPERTY    :5; //0x13
		uint8_t   GET_INT_STATUS  :5; //0x14
		uint8_t   PATCH_ARGS      :5; //0x15
		uint8_t   PATCH_DATA      :5; //0x16
		uint8_t   FM_TUNE_FREQ    :6; //0x20
		uint8_t   FM_SEEK_START   :6; //0x21
		uint8_t   FM_TUNE_STATUS  :6; //0x22
		uint8_t   FM_RSQ_STATUS   :6; //0x23
		uint8_t   FM_RDS_STATUS   :6; //0x24
		uint8_t   FM_AGC_STATUS   :6; //0x27
		uint8_t   FM_AGC_OVERRIDE :6; //0x28
		uint8_t   GPIO_CTL        :5; //0x80
		uint8_t   GPIO_SET        :5; //0x81
	}ADDR;

	struct{
		struct{
			union {
				struct
				{
					uint8_t   FUNC         :4;
				 	uint8_t   XOSCEN       :1;
				 	uint8_t   PATCH        :1;
				 	uint8_t   GPO2OEN      :1;
				 	uint8_t   CTSIEN       :1;
				 }Bit;
				 uint8_t Val;
			 }ARG1;
			 uint8_t OPMODE;
		}POWER_UP;
	}REG;

}xx;


#endif
