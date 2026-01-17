/*
 * DIX_usb.h
 *
 *  Created on: 12 ���. 2022 �.
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_DIX_USB_H_
#define EXCHANGE_INC_DIX_USB_H_

#define USB_STRING_RX_CIRLULYAR_BUF_SIZE     512
#define USB_STRING_RX_MAX_DATA_LEN           64
#define USB_STRING_PACKET_SEPARATOR_SIMBOL   0

#define USB_RX_BUFF_DATA_LEN                64
#define USB_RX_QUEUE_PACKETS_NUM            15

typedef struct
{
	uint8_t         Buff[USB_RX_QUEUE_PACKETS_NUM][USB_RX_BUFF_DATA_LEN];
	uint8_t         QueueFilling;
	uint8_t         TxQueuePointer;
	uint8_t         RxQueuePointer;
} sCirBuffUsbRx;

typedef struct
{
	uint8_t       UsbConnection;  // USB connection status to host
	uint8_t       ConnectionHid;  // USB connection status to host
	uint8_t       ConnectionCdc;  // USB connection status to host
	uint16_t      Send_Main_TimeOut;
	uint8_t       Send_other;  // Message delivery status to host, with additional USB endpoint
}sStateUSB ;

void     USB_Connected();
void     USB_Connected_UsbHid();
void     USB_Connected_UsbCdc();
void     USB_Disconnected();
void     USB_Sending_Completed(uint8_t EndPoint, uint8_t state);
void     Transmit_Data_Out_Delivered(uint8_t *Recive_USB);
void     Transmit_Data_Out_Pereferia(uint8_t* data, uint8_t dataLen, uint16_t timeDelay, uint8_t from, uint8_t crcCheck);
void     Transmit_Data_Out_No_Matter(uint8_t* data, uint8_t dataLen, uint16_t timeDelay, uint8_t from, uint8_t crcCheck);
//uint8_t  Transmit_Data_Out(uint8_t* data, uint16_t dataLen, uint16_t timeDelay, uint8_t from, uint8_t important);

uint8_t  Transmit_USB_CDC(uint8_t* Buf, uint16_t Len);
void     OnliSetNotification(uint8_t ControlByte);


uint8_t  USB_HID_Transmit_FS(uint8_t *dataBuff);
void     USB_Sending_Completed(uint8_t EndPoint, uint8_t state);
uint8_t  Transmit_Hid_Device(uint8_t *report, uint8_t size);


typedef struct  {
	uint8_t  id;       // 0x01
	uint8_t  modifiers;
    uint8_t  key1;
	uint8_t  key2;
	uint8_t  key3;
}keyboardHID_t;

//HID Media

typedef struct  {
    uint8_t  id;       // 0x02
    uint8_t  keys;
    uint8_t  noused;
}mediaHID_t;

//HID Mouse

typedef struct  {
	uint8_t  id;		// 0x03
    uint8_t  buttons;
    int8_t   x;
    int8_t   y;
    int8_t   wheel;
}mouseHID_t;

//HID Joystick

typedef struct  {
	uint8_t  id;		// 0x04
	uint8_t	 buttons;	// red + black
	uint8_t	 d_pad;		// lower 4 bits
	uint8_t	 gears;		// 1-7 (reverse)
	uint16_t axis[3];
}joystickHID_t;

//HID System Control

typedef struct {
    uint8_t  id;       // 0x05
    uint8_t  keys;
} SystemControlHID_t;

//HID Custom

typedef struct  {
	uint8_t  id;
	uint8_t	 data;
	uint8_t	 data1;
	uint8_t	 data2;
	uint8_t	 data3;
} CustomHID_t;

#endif /* EXCHANGE_INC_DIX_USB_H_ */
