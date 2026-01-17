#ifndef __EXCHANGE_H__
#define __EXCHANGE_H__
#include "main.h"

#include "Constants.h"
#include "DIX_uart.h"
#include "DIX_i2c.h"
#include "DIX_spi.h"
#include "DIX_usb.h"
#include "DIX_can.h"
#include "ExchangeConstant.h"

typedef union
{
    uint32_t ui32;
    uint8_t ui8[4];
} su32_to_u8;

typedef union
{
    uint32_t ui16;
    uint8_t ui8[2];
} su16_to_u8;

typedef struct
{
	sStateUSB USB;
	sStateI2C BusI2C1;
	sStateI2C BusI2C2;
}sExchangeState ;

typedef struct
{
	uint8_t        Code;
	uint32_t       Count_WR_OK;
	uint32_t       Count_WR_ERROR;
	uint32_t       Count_WR_BUSY;
	uint32_t       Count_WR_TIMEOUT;
	uint32_t       Count_WR_QUEUE_TIMEOUT;
	uint32_t       Count_WR_NO_CONNECTION;
	uint32_t       Count_WR_OTHER;
} sInterfaceCode;

typedef struct
{
	uint8_t        PresenceOfErrors;
	uint8_t        InterfacesInAction;
	uint32_t       Count_Total_WR_OK;
	uint32_t       Count_Total_WR_ERROR;
	uint32_t       Count_Total_WR_TIMEOUT;
	uint32_t       Count_Total_WR_BUSY;
	uint32_t       Count_Total_WR_QUEUE_TIMEOUT;
	uint32_t       Count_Total_WR_NO_CONNECTION;
	uint32_t       Count_Total_WR_OTHER;
	sInterfaceCode Interface[NumInterfaceCode];
} sDetailLogs;

typedef struct
{
	uint8_t       UsbConnection;
	uint8_t       DixomData_UsbSendState;
	uint8_t       PerederiaData_UsbSendState;
	uint8_t       UsbTx;
	uint8_t       UsbRx;
	uint8_t       I2C1Tx;
	uint8_t       I2C1Rx;
	uint8_t       I2C2Tx;
	uint8_t       I2C2Rx;
	uint8_t       SPITx;
	uint8_t       SPIRx;
	uint8_t       UART1Tx;
	uint8_t       UART1Rx;
	uint8_t       UART2Tx;
	uint8_t       UART2Rx;
	uint8_t       UART3Tx;
	uint8_t       UART3Rx;
	uint8_t       UART4Tx;
	uint8_t       UART4Rx;
	uint8_t       UART5Tx;
	uint8_t       UART5Rx;
	sDetailLogs   Detail;
} sExchangeLogs;

typedef struct
{
	sCirBuffUsbRx    UsbStringRx;
} sCircularBuffer;

/*--------Exchange Interface --------*/




typedef struct
{
	uint8_t         Current_interface;             // Current communication interface
	uint8_t         Interface_selection_mode;      // Interface selection method, 0 automatic, 1 manual
	uint8_t         Current_encoding;              // Current data encoding type
	uint8_t         Encoding_selection_mode;       // Encoding selection method, 0 automatic, 1 manual
	uint8_t         Manual_tx_usb_hid;             // sending data to the interface by manual selection 0 automatic. 1  manual
	uint8_t         Manual_tx_usb_cdc;             // sending data to the interface by manual selection 0 automatic. 1  manual
	uint8_t         Manual_tx_bluetooth;           // sending data to the interface by manual selection 0 automatic. 1  manual
	uint16_t        TimeutTimerUsbHidSend;         // If more than 500ms have elapsed since the last send, overhead data will be sent
} sInterface;



typedef struct
{
	sExchangeState   State;
	sExchangeLogs    Logs;
	sInterface       Interface;
	sCircularBuffer  CircularBuff;
} sExchange;

typedef struct
{
	uint8_t        USB;
	uint8_t        Bluetooth;
	uint8_t        Arduino;
	uint8_t        Nextion;
} sToSome;

typedef struct
{
	uint16_t        frameEncoding;
	uint8_t*        data;
	uint8_t         len;
} sDataOutputs;


typedef enum
{
	ExchangeMainSteam              = 0,
	ExchangeMainSteamDMA           = 1,
	ExchangeMemMainSteam           = 2,
	ExchangeMemMainSteamDMA        = 3,
	ExchangeInterrupt              = 4,
	ExchangeMemInterrupt           = 5,
	ExchangeSequentialIT           = 6,
	NoSemaphoreExchangeMainStream  = 7,
}Exchange_TypeTD;

typedef enum{
	longMessages,
	ShortMessages
}Exchange_Mode_UsbAudio;


#define MAX_CDC_MESSAGE_SIZE  255
#define MAX_SPP_MESSAGE_SIZE  128
#define MAX_HID_MESSAGE_SIZE  62
#define MAX_OUT_MESSAGE_SIZE  255

void       Exchange_Initialization(void);
WorkRes    ExchangeStatus(WorkRes State, uint8_t InterfaceCode);
WorkRes    TransmitDataOutputs(uint8_t* data, uint16_t dataLen,  boolean byteData,  boolean important, intreface intreface);
void       Write_ExchageLogs(uint8_t InterfaceCode, uint8_t ExchageState);
void       Copy_ArrayToArray(uint8_t from[], uint16_t fromBias, uint8_t to[],  uint16_t toBias, uint16_t count, uint16_t toMaxBuff);
void       Copy_Array(uint8_t *from[], uint16_t fromBias, uint8_t *to[],  uint16_t toBias, uint16_t count, uint16_t toMaxBuff);
uint32_t   Converter_8_to_u32(uint8_t* bytes);
void       Converter_u32_to_u8 (uint32_t u32, uint8_t* u8);
void       GATEWAY(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);
void       BIT_CHIP_CONTROL(uint8_t ControlByte, uint8_t *Recive_USB);

void       Regiser_Read_Loop();




#endif
