/**
  ******************************************************************************
  * @file    usbd_audio_core.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file provides the high layer firmware functions to manage the 
  *          following functionalities of the USB Audio Class:
  *           - Initialization and Configuration of high and low layer
  *           - Enumeration as Audio Streaming Device
  *           - Audio Streaming data transfer
  *           - AudioControl requests management
  *           - Error management
  *           
  *  @verbatim
  *      
  *          ===================================================================      
  *                                Audio Class Driver Description
  *          =================================================================== 
  *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Standard AC Interface Descriptor management
  *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *             - 1 Audio Streaming Endpoint
  *             - 1 Audio Terminal Input (1 channel)
  *             - Audio Class-Specific AC Interfaces
  *             - Audio Class-Specific AS Interfaces
  *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
  *             - Audio Feature Unit (limited to Mute control)
  *             - Audio Synchronization type: Asynchronous
  *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
  *          
  *           @note
  *            The Audio Class 1.0 is based on USB Specification 1.0 and thus supports only
  *            Low and Full speed modes and does not allow High Speed transfers.
  *            Please refer to "USB Device Class Definition for Audio Devices V1.0 Mar 18, 98"
  *            for more details.
  * 
  *           These aspects may be enriched or modified for a specific user application.
  *          
  *            This driver doesn't implement the following aspects of the specification 
  *            (but it is possible to manage these features with some modifications on this driver):
  *             - AudioControl Endpoint management
  *             - AudioControl requests other than SET_CUR and GET_CUR
  *             - Abstraction layer for AudioControl requests (only Mute functionality is managed)
  *             - Audio Synchronization type: Adaptive
  *             - Audio Compression modules and interfaces
  *             - MIDI interfaces and modules
  *             - Mixer/Selector/Processing/Extension Units (Feature unit is limited to Mute control)
  *             - Any other application-specific modules
  *             - Multiple and Variable audio sampling rates
  *             - Out Streaming Endpoint/Interface (microphone)
  *      
  *  @endverbatim
  *                                  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/

#include "usbd_audio_core.h"
//#include "usbd_audio_out_if.h"
#include "usbd_usr.h"
#include "usb_regs.h"
#include "usbd_ioreq.h"
//#include "stm32f4xx_dma.h"
//#include "stm32f4xx_spi.h"
//#include "stm32f4xx_tim.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx.h"
#include "main.h"
#include "stm32f411xe.h"
#include "SoundMenu.h"
//#include "lcd_s5230.h"
//#include "font16x20.h"
//#include "stm32f4xx_conf.h"

static volatile uint8_t flag = 0;
static volatile uint8_t dpid;

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
/* USER CODE BEGIN Private defines */

/** @defgroup usbd_audio 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup usbd_audio_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup usbd_audio_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup usbd_audio_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup usbd_audio_Private_FunctionPrototypes
  * @{
  */

/*********************************************
   AUDIO Device library callbacks
 *********************************************/
static uint8_t  usbd_audio_Init       (void *pdev, uint8_t cfgidx);
static uint8_t  usbd_audio_DeInit     (void *pdev, uint8_t cfgidx);
static uint8_t  usbd_audio_Setup      (void *pdev, USB_SETUP_REQ *req);
static uint8_t  usbd_audio_EP0_RxReady(void *pdev);
static uint8_t  usbd_audio_DataIn     (void *pdev, uint8_t epnum);
static uint8_t  usbd_audio_DataOut    (void *pdev, uint8_t epnum);
static uint8_t  usbd_audio_SOF        (void *pdev);
static uint8_t  usbd_audio_IN_Incplt  (void *pdev);
static uint8_t  usbd_audio_OUT_Incplt (void *pdev);

/*********************************************
   AUDIO Requests management functions
 *********************************************/
static void AUDIO_Req_GetCurrent      (void *pdev, USB_SETUP_REQ *req);
static void AUDIO_Req_SetCurrent      (void *pdev, USB_SETUP_REQ *req);
static uint8_t *USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length);
/**
  * @}
  */ 

/** @defgroup usbd_audio_Private_Variables
  * @{
  */ 

/* Main Buffer for Audio Control Rrequests transfers and its relative variables */
uint8_t  AudioCtl[64];
uint8_t  AudioCtlCmd = 0;
uint32_t AudioCtlLen = 0;
uint8_t  AudioCtlUnit = 0;
uint8_t  AudioCtlClass = 0;
uint8_t  AudioCtlFM = 0;

extern sUsbAudio UsbAudio;

#define AUDIO_BUFFER_SIZE           6144//(48 * 4 * 32)//32 �� ��� ������� 48���
#define SOF_RATE                    0x05
#define USB_BUFFER_SIZE_24          588//((96 + 2) * 2 * 3)
#define USB_BUFFER_SIZE_16          392//((96 + 2) * 2 * 2)
#define FB_RATE_MIN                 0x000a8000//(42 << 14)
#define FB_RATE_MAX                 0x00188000//(98 << 14)
#define SAMPLING_FREQ_MUTE_CONTROL  0x01
#define ENDPOINT_REQUEST_TYPE       0x02
#define RESERVED_MASK               ((uint32_t)0x0F7D0F7D)
#define GAP_L1                      500//AUDIO_BUFFER_SIZE*1/4
#define GAP_U1                      11200//AUDIO_BUFFER_SIZE*7/4
#define MAX_BLOCKSIZE	            (4608)
#define MAX_FRAMESIZE	            (22*1024)
#define BIT_RATE_16                 0
#define BIT_RATE_24                 1
#define BIT_RATE_32                 2

typedef union {
	uint16_t I2Sbuffer_16[4 * MAX_BLOCKSIZE];
	uint32_t I2Sbuffer_32[2 * MAX_BLOCKSIZE];
} I2SBUFF;

I2SBUFF Buff1;
I2SBUFF Buff2;

uint8_t FATbuffer[MAX_FRAMESIZE];
volatile uint32_t Sampling_freq __attribute__((aligned(4))) = 44100;
volatile uint32_t Sampling_freq_old __attribute__((aligned(4))) = 44100;
//uint8_t Speed = 0;
uint8_t mute = 0;
uint16_t num_byte;
volatile uint16_t num_samples;
volatile uint32_t FB_RATE __attribute__((aligned(4)))/* = (44 << 14) + (1 << 14) / 10*/;//��� ������� 44,1���
volatile uint8_t BIT_RATE;
volatile int gap;
volatile int gap_dev;
uint16_t old_gap;
volatile uint16_t num_remaining;
uint16_t num_remaining_old = 0;
volatile uint8_t spk_buffer_in = 0;
volatile uint8_t spk_buffer_out = 0;
volatile uint16_t spk_index = 0;
volatile uint8_t out_stop = 0;
volatile uint8_t PlayStart;
static __IO uint32_t  usbd_audio_AltSet = 0;
static uint8_t usbd_audio_CfgDesc[AUDIO_CONFIG_DESC_SIZE];
volatile uint32_t freq_in;
volatile uint8_t shift = 5;
volatile uint8_t freq_in_oled;

volatile uint8_t Oled_start;

char OLED_Freq[3] = {"441"};
char OLED_bitrate[2] = {"16"};



/* AUDIO interface class callbacks structure */
USBD_Class_cb_TypeDef  AUDIO_cb = 
{
  usbd_audio_Init,
  usbd_audio_DeInit,
  usbd_audio_Setup,
  NULL, /* EP0_TxSent */
  usbd_audio_EP0_RxReady,
  usbd_audio_DataIn,
  usbd_audio_DataOut,
  usbd_audio_SOF,
  usbd_audio_IN_Incplt,
  usbd_audio_OUT_Incplt,
  USBD_audio_GetCfgDesc,
#ifdef USB_OTG_HS_CORE
  USBD_audio_GetCfgDesc,
#endif

};

/* USB AUDIO device Configuration Descriptor */
static uint8_t usbd_audio_CfgDesc[AUDIO_CONFIG_DESC_SIZE] =
{
  /* Configuration 1 */
  USB_CONFIGUARTION_DESC_SIZE,          /* bLength              0x09*/
  USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType      0x02*/
  LOBYTE(AUDIO_CONFIG_DESC_SIZE),       /* wTotalLength  188 bytes*/
  HIBYTE(AUDIO_CONFIG_DESC_SIZE),      
  0x02,                                 /* bNumInterfaces       0x02*/
  0x01,                                 /* bConfigurationValue  0x01*/
  0x00,                                 /* iConfiguration       0x00*/
  0xC0,                                 /* bmAttributes  BUS Powred*/
  0x32,                                 /* bMaxPower = 100 mA*/
  /* 09 byte*/
  
  /* USB Speaker Standard interface descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength              0x09*/
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType      0x04*/
  0x00,                                 /* bInterfaceNumber     0x00*/
  0x00,                                 /* bAlternateSetting    0x00*/
  0x00,                                 /* bNumEndpoints        0x00*/
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass      0x01*/
  AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass   0x01*/
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol   0x00*/
  0x04,                                 /* iInterface           0x00*/
  /* 09 byte*/
  
  /* USB Speaker Class-specific AC Interface Descriptor */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength              0x09*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType      0x24*/
  AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype   0x01*/
  0x00,          /* 1.00 */             /* bcdADC               0x00*/
  0x01,                                 /*                      0x01*/
  0x28,                                 /* wTotalLength = 40    0x27*/
  0x00,                                 /*                      0x00*/
  0x01,                                 /* bInCollection        0x01*/
  0x01,                                 /* baInterfaceNr        0x01*/
  /* 09 byte*/
  
  /* USB Speaker Input Terminal Descriptor */
  AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength              0x0c*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType      0x24*/
  AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype   0x02*/
  0x01,                                 /* bTerminalID          0x01*/
  0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
  0x01,
  0x00,                                 /* bAssocTerminal       0x00*/
  0x02,                                 /* bNrChannels          0x02*/
  0x03,                                 /* wChannelConfig 0x0003  FL FR */
  0x00,
  0x00,                                 /* iChannelNames        0x00*/
  0x00,                                 /* iTerminal            0x00*/
  /* 12 byte*/
  
  /* USB Speaker Audio Feature Unit Descriptor */
  AUDIO_FEATURE_UNIT_DESC_SZ,           /* bLength              0x0a*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType      0x24*/
  AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype   0x06*/
  AUDIO_OUT_STREAMING_CTRL,             /* bUnitID              0x02*/
  0x01,                                 /* bSourceID            0x01*/
  0x01,                                 /* bControlSize         0x01*/
  0x01,    /*AUDIO_CONTROL_MUTE*/       /* bmaControls(0)       0x01*/
  0x00,                                 /* bmaControls(1)       0x00*/
  0x00,                                 /* bmaControls(2)       0x00*/
  0x00,                                 /* iTerminal            0x00*/
  /* 10 byte*/
  
  /*USB Speaker Output Terminal Descriptor */
  0x09,                                 /* bLength              0x09*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType      0x24*/
  AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype   0x03*/
  0x03,                                 /* bTerminalID          0x03*/
  0x01,                                 /* wTerminalType        0x0301*/
  0x03,
  0x00,                                 /* bAssocTerminal       0x00*/
  0x02,                                 /* bSourceID            0x02*/
  0x00,                                 /* iTerminal            0x02*/
  /* 09 byte*/
  
  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
  /* Interface 1, Alternate Setting 0                                             */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength              0x09*/
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType      0x04*/
  0x01,                                 /* bInterfaceNumber     0x01*/
  0x00,                                 /* bAlternateSetting    0x00*/
  0x00,                                 /* bNumEndpoints        0x00*/
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass      0x01*/
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass   0x02*/
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol   0x00*/
  0x05,                                 /* iInterface           0x00*/
  /* 09 byte*/
  
  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
  /* Interface 1, Alternate Setting 1                                           */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength               0x09*/
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType       0x04*/
  0x01,                                 /* bInterfaceNumber      0x01*/
  0x01,                                 /* bAlternateSetting     0x01*/
  0x02,                                 /* bNumEndpoints         0x02*/
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass       0x01*/
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass    0x02*/
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol    0x00*/
  0x05,                                 /* iInterface            0x00*/
  /* 09 byte*/
  
  /* USB Speaker Audio Streaming Interface Descriptor */
  AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength               0x07*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType       0x24*/
  AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype    0x01*/
  0x01,                                 /* bTerminalLink         0x01*/
  0x01,                                 /* bDelay                0x01*/
  0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
  0x00,
  /* 07 byte*/
  
  /* USB Speaker Audio Type III Format Interface Descriptor */
  0x0E,                                 /* bLength 20            0x14*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType       0x24*/
  AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype    0x02*/
  AUDIO_FORMAT_TYPE_I,                  /* bFormatType           0x01*/
  0x02,                                 /* bNrChannels           0x02*/
  0x02,                                 /* bSubFrameSize : 2 Bytes per frame (16bits) */
  0x10,                                 /* bBitResolution (16-bits per sample) */
  0x02,                                 /* bSamFreqType 4 frequency supported */
//  0x44,0xac,0x00,                       /* Audio sampling frequency 3 bytes 44100*/
  0x80,0xbb,0x00,                       /* Audio sampling frequency 3 bytes 48000*/
//  0x88,0x58,0x01,                       /* Audio sampling frequency 3 bytes 88200*/
  0x00,0x77,0x01,                       /* Audio sampling frequency 3 bytes 96000*/
  /* 20 byte*/

  /* Endpoint 1 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength                           0x09*/
  USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType                   0x05*/
  AUDIO_OUT_EP,                         /* bEndpointAddress 1 out endpoint   0x03*/
  0x05,                                 /* bmAttributes                      0x05*/
  LOBYTE(USB_BUFFER_SIZE_16),           /* wMaxPacketSize in Bytes (96 + 1) * 2(Stereo) * 2(16bit)) = 388*/
  HIBYTE(USB_BUFFER_SIZE_16),
  0x01,                                 /* bInterval                         0x01*/
  0x00,                                 /* bRefresh                          0x00*/
  AUDIO_IN_EP,                          /* bSynchAddress                     0x83*/
  /* 09 byte*/

  /* Endpoint - Audio Streaming Descriptor*/
  AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength                           0x07*/
  AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType                   0x25*/
  AUDIO_ENDPOINT_GENERAL,               /* bDescriptor                       0x01*/
  0x01,                                 /* bmAttributes                      0x01*/
  0x00,                                 /* bLockDelayUnits                   0x00*/
  0x00,                                 /* wLockDelay                        0x00*/
  0x00,                                                                    /*0x00*/
  /* 07 byte*/

  /* Endpoint 2 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength                           0x09*/
  USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType                   0x05*/
  AUDIO_IN_EP,                          /* bEndpointAddress 2 in endpoint    0x83*/
  0x11,        						    /* bmAttributes                      0x11*/
  0x03,0x00,   						    /* wMaxPacketSize in Bytes 3         0x03,0x00*/
  0x01,								    /* bInterval 1ms                     0x01*/
  SOF_RATE,							    /* bRefresh 32ms = 2^5               0x05*/
  0x00,                                 /* bSynchAddress                     0x00*/
  /* 09 byte*/

  /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
  /* Interface 1, Alternate Setting 2                                           */
  AUDIO_INTERFACE_DESC_SIZE,            /* bLength               0x09*/
  USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType       0x04*/
  0x01,                                 /* bInterfaceNumber      0x01*/
  0x02,                                 /* bAlternateSetting     0x02*/
  0x02,                                 /* bNumEndpoints         0x02*/
  USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass       0x01*/
  AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass    0x02*/
  AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol    0x00*/
  0x05,                                 /* iInterface            0x00*/
  /* 09 byte*/

  /* USB Speaker Audio Streaming Interface Descriptor */
  AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength               0x07*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType       0x24*/
  AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype    0x01*/
  0x01,                                 /* bTerminalLink         0x01*/
  0x01,                                 /* bDelay                0x01*/
  0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM  0x0001*/
  0x00,
  /* 07 byte*/

  /* USB Speaker Audio Type III Format Interface Descriptor */
  0x0E,                                 /* bLength 20            0x14*/
  AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType       0x24*/
  AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype    0x02*/
  AUDIO_FORMAT_TYPE_I,                  /* bFormatType           0x01*/
  0x02,                                 /* bNrChannels           0x02*/
  0x03,                                 /* bSubFrameSize : 3 Bytes per frame (24bits) */
  0x18,                                 /* bBitResolution (24-bits per sample) */
  0x02,                                 /* bSamFreqType 4 frequency supported */
 // 0x44,0xac,0x00,                       /* Audio sampling frequency 3 bytes 44100*/
  0x80,0xbb,0x00,                       /* Audio sampling frequency 3 bytes 48000*/
 // 0x88,0x58,0x01,                       /* Audio sampling frequency 3 bytes 88200*/
  0x00,0x77,0x01,                       /* Audio sampling frequency 3 bytes 96000*/
  /* 20 byte*/

  /* Endpoint 1 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength                          0x09*/
  USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType                  0x05*/
  AUDIO_OUT_EP,                         /* bEndpointAddress 1 out endpoint  0x03*/
  0x05,                                 /* bmAttributes                     0x05*/
  LOBYTE(USB_BUFFER_SIZE_24),           /* wMaxPacketSize in Bytes (96 + 1) * 2(Stereo) * 3(24bit)) = 582 */
  HIBYTE(USB_BUFFER_SIZE_24),
  0x01,                                 /* bInterval                        0x01*/
  0x00,                                 /* bRefresh                         0x00*/
  AUDIO_IN_EP,                          /* bSynchAddress                    0x83*/
  /* 09 byte*/

  /* Endpoint - Audio Streaming Descriptor*/
  AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength                          0x07*/
  AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType                  0x25*/
  AUDIO_ENDPOINT_GENERAL,               /* bDescriptor                      0x01*/
  0x01,                                 /* bmAttributes                     0x01*/
  0x00,                                 /* bLockDelayUnits                  0x00*/
  0x00,                                 /* wLockDelay                       0x00*/
  0x00,                                                                   /*0x00*/
  /* 07 byte*/

  /* Endpoint 2 - Standard Descriptor */
  AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength                          0x09*/
  USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType                  0x05*/
  AUDIO_IN_EP,                          /* bEndpointAddress 2 in endpoint   0x83*/
  0x11,        						    /* bmAttributes                     0x11*/
  0x03,0x00,   						    /* wMaxPacketSize in Bytes 3        0x03,0x00*/
  0x01,								    /* bInterval 1ms                    0x01*/
  SOF_RATE,							    /* bRefresh 32ms = 2^5              0x05*/
  0x00,                                 /* bSynchAddress                    0x00*/
  /* 09 byte*/
} ;

//uint32_t feedback_data;
//uint32_t FB_nom=(44<<14);
//uint8_t OUT_num=0;
//uint8_t IN_num=0;

/**
  * @}
  */ 

/** @defgroup usbd_audio_Private_Functions
  * @{
  */ 

/**
* @brief  usbd_audio_Init
*         Initilaizes the AUDIO interface.
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
uint8_t InitState = 0;
static uint8_t  usbd_audio_Init (void  *pdev, 
                                 uint8_t cfgidx)
{  
	  /* Open EP OUT */
	 DCD_EP_Open(pdev,
	             AUDIO_OUT_EP,
	             USB_BUFFER_SIZE_24,
	             USB_OTG_EP_ISOC);

	 /* Open EP IN */
	 DCD_EP_Open(pdev,
			  	 AUDIO_IN_EP,
	             3,
	             USB_OTG_EP_ISOC);

	 DCD_EP_Flush(pdev,AUDIO_IN_EP);

	  /* Initialize the Audio output Hardware layer */
//	  if (AUDIO_OUT_fops.Init(USBD_AUDIO_FREQ) != USBD_OK)
//	  {
//		  return USBD_FAIL;
//	  }

	 if(InitState == 0){
			SPI2->I2SCFGR &= 0xf040;
			SPI2->I2SCFGR |= (SPI_I2SCFGR_I2SMOD | I2S_MODE_MASTER_TX | I2S_STANDARD_PHILIPS |
							  I2S_DATAFORMAT_24B | I2S_CPOL_LOW);

			SPI2->I2SPR = 0x0004;//��� ������� 44,1 � 48 �������� SPI = 4

			SPI2->CR2 |= (uint16_t)SPI_CR2_TXDMAEN;//��������� I2S/DMA

			//    I2S_Cmd(SPI2, ENABLE);
			SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;
			InitState = 1;
	 }


	 flag = 1;
	 spk_buffer_in = 0;
	 spk_buffer_out = 0;
	 UsbAudio.State.Playback = 0;
	 spk_index = 0;
	 out_stop = 0;
	 PlayStart = 0;
//	 GEN2_OFF;
//	 GEN1_ON;

	 return USBD_OK;
}

/**
* @brief  usbd_audio_DeInit
*         DeInitializes the AUDIO layer.
* @param  pdev: device instance
* @param  cfgidx: Configuration index
* @retval status
*/
static uint8_t  usbd_audio_DeInit (void  *pdev, 
                                   uint8_t cfgidx)
{ 
	  DCD_EP_Flush (pdev, AUDIO_OUT_EP);
	  DCD_EP_Flush (pdev, AUDIO_IN_EP);
	  DCD_EP_Close (pdev, AUDIO_OUT_EP);
	  DCD_EP_Close (pdev, AUDIO_IN_EP);
	  flag = 0;

	  /* DeInitialize the Audio output Hardware layer */
//	  if (AUDIO_OUT_fops.DeInit(0) != USBD_OK)
//	  {
//	    return USBD_FAIL;
//	  }
	  /* Stop the Transfer on the I2S side: Stop and disable the DMA stream */
	DMA1_Stream4->CR &= ~(uint32_t)DMA_SxCR_EN;//������������� DMA

	  /* Clear all the DMA flags for the next transfer */
	DMA1->HIFCR = (uint32_t)((DMA_HISR_TCIF4 | DMA_HISR_HTIF4 | DMA_HISR_FEIF4 |
	    					  DMA_HISR_TEIF4) & RESERVED_MASK);

	SPI2->CR2 &= ~(uint16_t)SPI_CR2_TXDMAEN;//��������� I2S/DMA

	while (((SPI2->SR & (uint16_t)SPI_SR_TXE) == 0) || (SPI2->SR & (uint16_t)SPI_SR_BSY))
	{
	}
	//I2S_Cmd(SPI2, DISABLE);
	SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SE;

	return USBD_OK;
}

/**
  * @brief  usbd_audio_Setup
  *         Handles the Audio control request parsing.
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  usbd_audio_Setup (void  *pdev, 
                                  USB_SETUP_REQ *req)
{
	  uint16_t len;
	  uint8_t  *pbuf;
	//  uint32_t tmp;

	  switch (req->bmRequest & USB_REQ_TYPE_MASK)
	  {
	    /* AUDIO Class Requests -------------------------------*/
	  	  case USB_REQ_TYPE_CLASS :
	  		  switch (req->bRequest)
	  		  {
	  		  	  case AUDIO_REQ_GET_CUR:
	  		  		  AUDIO_Req_GetCurrent(pdev, req);
	  		  		  break;

	  		  	  case AUDIO_REQ_SET_CUR:
	  		  		  AUDIO_Req_SetCurrent(pdev, req);
	  		  		  break;

	  		  	  default:
	  		  		  USBD_CtlError (pdev, req);
	  		  		  return USBD_FAIL;
	  		  }
	  		  break;

	    /* Standard Requests -------------------------------*/
	  	  case USB_REQ_TYPE_STANDARD:
	  		  switch (req->bRequest)
	  		  {
	  		  	  case USB_REQ_GET_DESCRIPTOR:
	  		  		  if ( (req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
	  		  		  {
	  		  			  pbuf = usbd_audio_CfgDesc + 18;
	  		  			  len = MIN(USB_AUDIO_DESC_SIZ , req->wLength);
	  		  		  }
	  		  		  USBD_CtlSendData (pdev,
	  		  				  	  	  	pbuf,
	                                    len);
	  		  		  break;

	  		  	  case USB_REQ_GET_INTERFACE :
	  		  		  USBD_CtlSendData (pdev,
	                                   (uint8_t *)&usbd_audio_AltSet,
	                                    1);
	  		  		  break;

	  		  	  case USB_REQ_SET_INTERFACE :
	  		  		  if ((uint8_t)(req->wValue) <= AUDIO_TOTAL_IF_NUM)
	  		  		  {
	  		  			  usbd_audio_AltSet = (uint8_t)(req->wValue);
	  		  			  if (usbd_audio_AltSet == 1)
	  		  			  {
	  		  				  //SOF_num = 0;
	  		  				  BIT_RATE = BIT_RATE_16;
	  		  				  OLED_bitrate[0] = 0x31;
	  		  				  OLED_bitrate[1] = 0x36;
	  		  				 // UsbConState(1);
	  		  				 // SetBitRate(1);
	  		  				UsbAudio.State.BitRate = BIT_16;
	  		  				  //LED_BLUE_OFF;
	  		  			  }
	  		  			  if (usbd_audio_AltSet == 2)
	  		  			  {
	  		  				  //SOF_num = 0;
	  		  				  BIT_RATE = BIT_RATE_24;
	  		  				  OLED_bitrate[0] = 0x32;
	  		  				  OLED_bitrate[1] = 0x34;
	  		  				//  SetBitRate(2);
	  		  				 // UsbConState(1);
	  		  				UsbAudio.State.BitRate = BIT_24;
	  		  				  //LED_BLUE_ON;
	  		  			  }
	  		  			  UsbAudio.State.USB = Connected;
  		  				  flag = 0;
  		  				  DCD_EP_Flush(pdev, AUDIO_IN_EP);
  		  				  Oled_start = 1;
	  		  		  }
	  		  		  else
	  		  		  {
	  		  			  /* Call the error management function (command will be nacked */
	  		  			  USBD_CtlError (pdev, req);
	  		  		  }
	  		  		  break;
	  		  }
	  }
	  return USBD_OK;
}

/**
  * @brief  usbd_audio_DataIn
  *         Handles the audio IN data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_audio_DataIn (void *pdev, uint8_t epnum)
{
	if (epnum == (AUDIO_IN_EP & 0x7f))
	{
		flag = 0;
//		SOF_num=0;
		//DCD_EP_Tx (pdev, AUDIO_IN_EP, (uint8_t *) &FB_RATE, 3);
	}
	return USBD_OK;
}

/**
  * @brief  usbd_audio_DataOut
  *         Handles the Audio Out data stage.
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */
static uint8_t  usbd_audio_DataOut (void *pdev, uint8_t epnum)
{     
	if (epnum == AUDIO_OUT_EP)
	{
		num_byte = USBD_GetRxCount (pdev, epnum);
        //��������� ������ �������� �����
	    DCD_EP_PrepareRx(pdev,
	                     AUDIO_OUT_EP,
	                    (uint8_t*)FATbuffer,
	                     num_byte);
        /* Toggle the frame index */
		((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame =
		(((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame)? 0:1;
  	    out_stop = 1;
	}
return USBD_OK;
}

/**
  * @brief  usbd_audio_SOF
  *         Handles the SOF event (data buffer update and synchronization).
  * @param  pdev: instance
  * @param  epnum: endpoint number
  * @retval status
  */

uint8_t updateAudioData = 0;
static uint8_t  usbd_audio_SOF (void *pdev)
{
	updateAudioData = 1;
	USB_OTG_DSTS_TypeDef  FS_DSTS;
	uint16_t k, l;
	uint8_t byte_per_sampl;
//	TIM8->CR1 &= ~TIM_CR1_CEN;
//	freq_in = TIM8->CNT;
//	TIM8->CNT = 0;
//	TIM8->CR1 |= TIM_CR1_CEN;
	freq_in = TIM2->CCR1;
	//Oled_start = 1;
	//freq_in_oled = freq_in & 0x0000ffff;
	num_remaining = (DMA1_Stream4->NDTR);
	if ((num_remaining != num_remaining_old) || (PlayStart == 0))
	{
    	if (PlayStart)
    	{
    		if (spk_buffer_in == spk_buffer_out)
        	{
        		if (spk_index < (AUDIO_BUFFER_SIZE - num_remaining))
        		{
        			gap = AUDIO_BUFFER_SIZE - spk_index - num_remaining;
        		}
        		else
        		{
    				gap = (AUDIO_BUFFER_SIZE - spk_index) + (AUDIO_BUFFER_SIZE - num_remaining) + AUDIO_BUFFER_SIZE;
    			}
        	}
        	else
    		{
        		gap = (AUDIO_BUFFER_SIZE - spk_index) + (AUDIO_BUFFER_SIZE - num_remaining);
    		}
    	}
    	//gap_out = gap;
        if (BIT_RATE == BIT_RATE_24) byte_per_sampl = 6;
        else byte_per_sampl = 4;
        num_samples = num_byte / byte_per_sampl;
//    	SOF_num++;
		if (out_stop)
		{
			//���������� �������� �����
//			if (SOF_num == 32)
//			{
//				if (gap > AUDIO_BUFFER_SIZE)
//				{
//					if /*(FB_RATE < FB_RATE_MAX)*/(gap > old_gap) gap_dev += ((gap - AUDIO_BUFFER_SIZE) / 64) & 0xfff0;
//				}
//				else
//				{
//					if /*((FB_RATE > FB_RATE_MIN)*/(gap < old_gap) gap_dev -= ((AUDIO_BUFFER_SIZE - gap) / 64) & 0xfff0;
//				}
//			}
			FB_RATE = (freq_in << (shift)) + ((gap - AUDIO_BUFFER_SIZE) / 8)/* + gap_dev*/;
	    	//������������ ������ � �������� �����������
	    	l = 0;
	    	for (k = 0; k < num_samples; k++)
	    	{
		    	//���� gap ������� ����, �� ���������� ������ � �����������
	    		if ((gap > GAP_L1) || (PlayStart == 0))
		    	{
		    		if (spk_buffer_in == 0)
		    		{
		    			if (BIT_RATE == BIT_RATE_16)
		    			{
		    				Buff1.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 1] << 8) | ((uint16_t)FATbuffer[l]);
		    				Buff1.I2Sbuffer_16[spk_index++] = 0;
		    				Buff1.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 3] << 8) | ((uint16_t)FATbuffer[l + 2]);
		    				Buff1.I2Sbuffer_16[spk_index++] = 0;
		    			}
		    			else
		    			{
		    				Buff1.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 2] << 8) | ((uint16_t)FATbuffer[l + 1]);
		    				Buff1.I2Sbuffer_16[spk_index++] = (uint16_t)FATbuffer[l];
		    				Buff1.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 5] << 8) | ((uint16_t)FATbuffer[l + 4]);
		    				Buff1.I2Sbuffer_16[spk_index++] = (uint16_t)FATbuffer[l + 3] << 8;
		    			}
		    		}
		    		else
		    		{
		    			if (BIT_RATE == BIT_RATE_16)
		    			{
		    				Buff2.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 1] << 8) | ((uint16_t)FATbuffer[l]);
		    				Buff2.I2Sbuffer_16[spk_index++] = 0;
		    				Buff2.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 3] << 8) | ((uint16_t)FATbuffer[l + 2]);
		    				Buff2.I2Sbuffer_16[spk_index++] = 0;
		    			}
		    			else
		    			{
		    				Buff2.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 2] << 8) | ((uint16_t)FATbuffer[l + 1]);
		    				Buff2.I2Sbuffer_16[spk_index++] = (uint16_t)FATbuffer[l];
		    				Buff2.I2Sbuffer_16[spk_index++] = ((uint16_t)FATbuffer[l + 5] << 8) | ((uint16_t)FATbuffer[l + 4]);
		    				Buff2.I2Sbuffer_16[spk_index++] = (uint16_t)FATbuffer[l + 3] << 8;
		    			}
		    		}
		    		l += byte_per_sampl;
		    	}
				if (spk_index >= AUDIO_BUFFER_SIZE)
				{
					spk_index = 0;
					spk_buffer_in = 1 - spk_buffer_in;
					/* Trigger the start of streaming only when spk_buffer_0 is full */
					if ((PlayStart == 0) && (spk_buffer_in == 1))
					{
						/* Enable start of Streaming */
						PlayStart = 1;
						//AUDIO_OUT_fops.AudioCmd((uint32_t*)&Audio_buf_0,        /* Samples buffer pointer */
				                             	// AUDIO_BUFFER_SIZE,              /* Number of samples in Bytes */
				                             	 //AUDIO_CMD_PLAY);                /* Command to be processed */
						/* Disable the I2S DMA Stream*/
					    DMA1_Stream4->CR &= ~(uint32_t)DMA_SxCR_EN;
					    while ((DMA1_Stream4->CR & DMA_SxCR_EN) != 0)
						{
						}
						/* Clear all the DMA flags for the next transfer */
						DMA1->HIFCR = (uint32_t)((DMA_HISR_TCIF4 | DMA_HISR_HTIF4 | DMA_HISR_FEIF4 |
						    					  DMA_HISR_TEIF4) & RESERVED_MASK);
			        	DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
			        	DMA1_Stream4->M0AR = (uint32_t)&Buff1.I2Sbuffer_16[0];
			    	    //DMA_Cmd(DMA1_Stream4, ENABLE);
			    		DMA1_Stream4->CR |= (uint32_t)DMA_SxCR_EN;
					}

				}
	    	}
	    	//���� gap ������� ������, �� ���������� � ����������� �������������� ������
	    	if (gap > GAP_U1)
	    	{
				num_samples = (freq_in >> (14 - shift)) + 2;
				for (k = 0; k < num_samples; k++)
				{
					if (spk_buffer_in == 0)
					{
						Buff1.I2Sbuffer_16[spk_index++] = 0;
						Buff1.I2Sbuffer_16[spk_index++] = 0;
						Buff1.I2Sbuffer_16[spk_index++] = 0;
						Buff1.I2Sbuffer_16[spk_index++] = 0;
					}
					else
					{
						Buff2.I2Sbuffer_16[spk_index++] = 0;
						Buff2.I2Sbuffer_16[spk_index++] = 0;
						Buff2.I2Sbuffer_16[spk_index++] = 0;
						Buff2.I2Sbuffer_16[spk_index++] = 0;
					}
					if (spk_index >= AUDIO_BUFFER_SIZE)
					{
						spk_index = 0;
						spk_buffer_in = 1 - spk_buffer_in;
					}
				}
	    	}
	    	old_gap = gap;
		}
		else 	//���� ������� OUT ���, �� ���������� � ���������� ������� ��������
		{
			num_samples = freq_in >> (14 - shift);
			if (gap > (AUDIO_BUFFER_SIZE + 10))
			{
				if (gap >AUDIO_BUFFER_SIZE + 20) num_samples += 2;
				else num_samples += 1;
			}
			else if (gap < (AUDIO_BUFFER_SIZE - 10))
			{
				if (gap < (AUDIO_BUFFER_SIZE - 20)) num_samples -= 2;
				else num_samples -= 1;
			}
			for (k = 0; k < num_samples; k++)
			{
				if (spk_buffer_in == 0)
				{
					Buff1.I2Sbuffer_16[spk_index++] = 0;
					Buff1.I2Sbuffer_16[spk_index++] = 0;
					Buff1.I2Sbuffer_16[spk_index++] = 0;
					Buff1.I2Sbuffer_16[spk_index++] = 0;
				}
				else
				{
					Buff2.I2Sbuffer_16[spk_index++] = 0;
					Buff2.I2Sbuffer_16[spk_index++] = 0;
					Buff2.I2Sbuffer_16[spk_index++] = 0;
					Buff2.I2Sbuffer_16[spk_index++] = 0;
				}
				if (spk_index >= AUDIO_BUFFER_SIZE)
				{
					spk_index = 0;
					spk_buffer_in = 1 - spk_buffer_in;
				}
			}
//    		FB_LED_DOWN_OFF;
//    		FB_LED_UP_OFF;
		}
	}
	out_stop = 0;
    num_remaining_old = num_remaining;
	if ((!flag))
	{
		FS_DSTS.d32 = USB_OTG_READ_REG32(&(((USB_OTG_CORE_HANDLE*)pdev)->regs.DREGS->DSTS));
		if (((FS_DSTS.b.soffn)&0x1) == dpid)
		{
			DCD_EP_Tx (pdev, AUDIO_IN_EP, (uint8_t *) &FB_RATE, 3);
			flag = 1;
		}
	}
//	if (SOF_num >= 128) SOF_num = 0;
	return USBD_OK;
}

/**
  * @brief  usbd_audio_IN_Incplt
  *         Handles the iso out incomplete event.
  * @param  pdev: instance
  * @retval status
  */
static uint8_t  usbd_audio_IN_Incplt (void  *pdev)
{
	//This ISR is executed every time when IN token received with "wrong" PID. It's necessary
	//to flush IN EP (feedback EP), get parity value from DSTS, and store this info for SOF handler.
	//SOF handler should skip one frame with "wrong" PID and attempt a new transfer a frame later.

	USB_OTG_DSTS_TypeDef  FS_DSTS;
	FS_DSTS.d32 = USB_OTG_READ_REG32(&(((USB_OTG_CORE_HANDLE*)pdev)->regs.DREGS->DSTS));
	dpid=(FS_DSTS.b.soffn)&0x1;
	if (flag)
	{
		flag = 0;
		DCD_EP_Flush(pdev, AUDIO_IN_EP);
	}
	return USBD_OK;
}

/**
  * @brief  usbd_audio_OUT_Incplt
  *         Handles the iso out incomplete event.
  * @param  pdev: instance
  * @retval status
  */
static uint8_t  usbd_audio_OUT_Incplt (void  *pdev)
{
	return USBD_OK;
}

/******************************************************************************
     AUDIO Class requests management
******************************************************************************/
/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req)
{  
    if (HIBYTE(req->wValue) == SAMPLING_FREQ_MUTE_CONTROL)//���� ������� SAMPLING_FREQ_CONTROL ��� MUTE
    {
    	if ((req->bmRequest & 0x0f) == ENDPOINT_REQUEST_TYPE)//���� ������ � �������� �����
    	{
        	AudioCtl[0] = (uint8_t)(Sampling_freq & 0xff);   //�� �������� �������� �������
        	AudioCtl[1] = (uint8_t)((Sampling_freq>>8) & 0xff);
        	AudioCtl[2] = (uint8_t)((Sampling_freq>>16) & 0xff);
        	/* Send the current frequency state */
            USBD_CtlSendData (pdev,
                              AudioCtl,
                              3);
    	}
    	else if (HIBYTE(req->wIndex) == AUDIO_OUT_STREAMING_CTRL)//����� ��� ������� MUTE
    	{
    		AudioCtl[0] = mute;
    		/* Send the current mute state */
    	    USBD_CtlSendData (pdev,
    	                      AudioCtl,
    	                      1);
    	}
    }
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req)
{
	//GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
	if (req->wLength)
	  {
	    /* Prepare the reception of the buffer over EP0 */
	    USBD_CtlPrepareRx (pdev,
	                       AudioCtl,
	                       req->wLength);

	    /* Set the global variables indicating current request and its length
	    to the function usbd_audio_EP0_RxReady() which will process the request */
	    AudioCtlCmd = AUDIO_REQ_SET_CUR;        /* Set the request value */
	    AudioCtlLen = req->wLength;             /* Set the request data length */
	    AudioCtlUnit = HIBYTE(req->wIndex);     /* Set the request target unit */
	    AudioCtlClass = req->bmRequest & 0x0f;  /* Set the Class Requests*/
	    AudioCtlFM = HIBYTE(req->wValue);
	  }
}

/**
  * @brief  usbd_audio_EP0_RxReady
  *         Handles audio control requests data.
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  usbd_audio_EP0_RxReady (void  *pdev)
{
	/* Check if an AudioControl request has been issued */
	if (AudioCtlCmd == AUDIO_REQ_SET_CUR)
	{/* In this driver, to simplify code, only SET_CUR request is managed */
		if (AudioCtlFM == SAMPLING_FREQ_MUTE_CONTROL)//���� ������� SAMPLING_FREQ_CONTROL ��� MUTE
		{
			if (AudioCtlClass == ENDPOINT_REQUEST_TYPE)//���� ������ � �������� �����
			{
				//��������� �������� �������
				Sampling_freq = (*(uint32_t *)&AudioCtl[0]) & 0x00ffffff;
				switch (Sampling_freq)
				{
					case 44100 :
					{
						//SetSamplingFreq(1);
						UsbAudio.State.SamplingFreq = SFreq_44100;
					};break;
					case 48000 :
					{
					//	SetSamplingFreq(2);
						UsbAudio.State.SamplingFreq = SFreq_48000;
					};break;
					case 88200 :
					{
						//SetSamplingFreq(3);
						UsbAudio.State.SamplingFreq = SFreq_88200;
					};break;
					case 96000 :
					{
						//SetSamplingFreq(4);
						UsbAudio.State.SamplingFreq = SFreq_96000;
					}
					//UsbConState(1);
					UsbAudio.State.USB = Connected;
				}

				if (Sampling_freq_old != Sampling_freq)
				{
					//���������� �������� ������������ � ����� FB_RATE ��� ������ ������
					switch (Sampling_freq)
					{
						case 44100 :
						{
							SPI2->I2SPR = 0x0004;//����������� ������� I2S = 4
							//GEN2_OFF;
							//GEN1_ON;
							shift = 5;
						};break;
						case 48000 :
						{
							SPI2->I2SPR = 0x0004;//����������� ������� I2S = 4
							//GEN1_OFF;
							//GEN2_ON;
							shift = 5;
						};break;
						case 88200 :
						{
							SPI2->I2SPR = 0x0002;//����������� ������� I2S = 2
							//GEN2_OFF;
							//GEN1_ON;
							shift = 6;
						};break;
						case 96000 :
						{
							SPI2->I2SPR = 0x0002;//����������� ������� I2S = 2
							//GEN1_OFF;
							//GEN2_ON;
							shift = 6;
						}
					}
					gap_dev = 0;
					//PlayStart = 0;
					spk_buffer_in = 1 - spk_buffer_out;
					//spk_buffer_in = 0;
					//spk_buffer_out = 0;
					spk_index = (AUDIO_BUFFER_SIZE - num_remaining)&0xfffc;
					//spk_index = 0;
					num_remaining_old = num_remaining;
					Oled_start = 1;
				}
				Sampling_freq_old = Sampling_freq;
			}
			else if (AudioCtlUnit == AUDIO_OUT_STREAMING_CTRL)//����� ��� ������� MUTE
			{
				mute = AudioCtl[0];
	//    			if (mute) GPIO_SetBits(GPIOB,GPIO_Pin_4);
	//    			else GPIO_ResetBits(GPIOB,GPIO_Pin_4);
			}
		}
	    AudioCtlCmd = 0;
	    AudioCtlLen = 0;
	    AudioCtlFM = 0;
	    AudioCtlClass = 0;
	}
	return USBD_OK;
}

/**
  * @brief  USBD_audio_GetCfgDesc 
  *         Returns configuration descriptor.
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_audio_GetCfgDesc (uint8_t speed, uint16_t *length)
{
	*length = sizeof (usbd_audio_CfgDesc);
	return usbd_audio_CfgDesc;
}

void Audio_Handler_device(void)
{
    /* Disable the I2S DMA Stream*/
    //DMA_Cmd(DMA1_Stream4, DISABLE);
    DMA1_Stream4->CR &= ~(uint32_t)DMA_SxCR_EN;

    /* Clear the Interrupt flag */
    //DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF7 |DMA_FLAG_HTIF7 | DMA_FLAG_FEIF7 | DMA_FLAG_TEIF7);
	DMA1->HIFCR = (uint32_t)((DMA_HISR_TCIF4 | DMA_HISR_HTIF4 | DMA_HISR_FEIF4 |
	    					  DMA_HISR_TEIF4) & RESERVED_MASK);
    while ((DMA1_Stream4->CR & DMA_SxCR_EN) != 0)
	{
	}

    if (PlayStart)
    {
    	if(updateAudioData == 1){
        	if (spk_buffer_out == 0)
        	{
            		//AUDIO_OUT_fops.AudioCmd((uint32_t*)&Audio_buf_1,         /* Samples buffer pointer */
                                       	    //AUDIO_BUFFER_SIZE,               /* Number of samples in Bytes */
                                       	    //AUDIO_CMD_PLAY);                 /* Command to be processed */
            	DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
            	DMA1_Stream4->M0AR = (uint32_t)&Buff2.I2Sbuffer_16[0];
            	updateAudioData = 0;
        	}
        	else
        	{
            		//AUDIO_OUT_fops.AudioCmd((uint32_t*)&Audio_buf_0,         /* Samples buffer pointer */
                                       	    //AUDIO_BUFFER_SIZE,               /* Number of samples in Bytes */
                                       	    //AUDIO_CMD_PLAY);                 /* Command to be processed */
            	DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
            	DMA1_Stream4->M0AR = (uint32_t)&Buff1.I2Sbuffer_16[0];
            	updateAudioData = 0;
        	}

    	}else{

    		for ( uint16_t i = 0; i<  4 * MAX_BLOCKSIZE; i++){
    			Buff1.I2Sbuffer_16[i] = 0;
    			Buff2.I2Sbuffer_16[i] = 0;


    		}
    		DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
    		DMA1_Stream4->M0AR = (uint32_t)&Buff1.I2Sbuffer_16[0];

    		DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
    	    DMA1_Stream4->M0AR = (uint32_t)&Buff2.I2Sbuffer_16[0];


    		// for () Buff2.I2Sbuffer_16


//        	DMA1_Stream4->NDTR = (uint32_t)AUDIO_BUFFER_SIZE;
//        	DMA1_Stream4->M0AR = (uint32_t)0;
    	}

	    //DMA_Cmd(DMA1_Stream4, ENABLE);
		DMA1_Stream4->CR |= (uint32_t)DMA_SxCR_EN;
    	spk_buffer_out = 1 - spk_buffer_out;


    	if(Buff1.I2Sbuffer_16[0] != 0 || Buff1.I2Sbuffer_16[100] != 0 || Buff2.I2Sbuffer_16[0] != 0 || Buff2.I2Sbuffer_16[100] != 0){
    		UsbAudio.State.Playback = 1;
    	}else{
    		UsbAudio.State.Playback = 0;
    	}


    }
    else
    {
		//AUDIO_OUT_fops.AudioCmd((uint32_t*)&Audio_buf_0,         /* Samples buffer pointer */
                           	    //AUDIO_BUFFER_SIZE,               /* Number of samples in Bytes */
                           	    //AUDIO_CMD_STOP);                 /* Command to be processed */
		spk_buffer_out = 0;
		UsbAudio.State.Playback = 0;
	}
}
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
