/*
 * Constant.h
 *
 *  Created on: May 8, 2022
 *      Author: 79825
 */

#ifndef EXCHANGE_INC_EXCHANGECONSTANT_H_
#define EXCHANGE_INC_EXCHANGECONSTANT_H_


typedef enum{
	UTF8_WHOLE        = 33, // '!',
	UTF8_PART_START   = 34, // '"',
	UTF8_PART         = 35, // '#',
	UTF8_PART_END     = 36, // '$',

	BASE64_WHOLE      = 37, // '%',
	BASE64_PART_START = 38, // '&',
	BASE64_PART       = 39, // '\'',
	BASE64_PART_END   = 40, // '(',

	BYTE_WHOLE        = 41, // ')',
	BYTE_PART_START   = 42, // '*',
	BYTE_PART         = 43, // '+',
	BYTE_PART_END     = 44, // ',',
}FrameEncoding;


typedef enum
{
	deccrement  = 0x00U,
	increment   = 0x01U,
	reset       = 0x02U,
} operation;

typedef enum
{
	false     = 0x00U,
	true      = 0x01U,
	unknown   = 0x02U,
} boolean;


typedef enum
{
	ASCII,
	BYTE,
	ASCII_HEX,
	ASCII_BYTE,
	ASCII_BIN
}eEncoding;

typedef enum
{
	usbHid                       = 0x00U, //usb
	USB_CDC                      = 0x01U, //usb
	BLUETOOTH_SPP                = 0x02U, //uart 1
	BLUETOOTH_GATT               = 0x03U, //uart 1
	ARDUINO                      = 0x04U, //uart 3
	NEXTION                      = 0x05U, //uart 5
	RF_MODULE                    = 0x06U, //uart 2
	allAvailableExceptCurrent    = 0xFDU, //Sending to everything except the current one
	allAvailableForced           = 0xFEU, //Sending will be carried out on all connected interfaces. Regardless of interface settings.
	allAvailable                 = 0xFFU, //Sending depending on the selected interface or the settings of the interface itself

}intreface;









#endif
