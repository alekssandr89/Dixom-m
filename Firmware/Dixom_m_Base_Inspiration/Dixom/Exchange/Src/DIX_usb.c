/*
 * DIX_usb.c
 *
 *  Created on: Mar 11, 2022
 *      Author: 79825
 */
#include "stdint.h"
#include "Constants.h"
#include "Structures.h"
#include "Exchange.h"
#include "cmsis_os.h"
#include "DIX_usb.h"
#include "Functions.h"
#include <base64.h>
#include <Functions.h>

#include "../../../Usb/COMPOSITE/App/usb_device.h"
#include "../../../Usb/COMPOSITE/App/usbd_cdc_acm_if.h"
#include "../../../Usb/COMPOSITE/Class/HID_CUSTOM/Inc/usbd_hid_custom.h"
#include "../../../Usb/COMPOSITE/Class/HID_KEYBOARD/Inc/usbd_hid_keyboard.h"

extern USBD_HandleTypeDef hUsbDevice;
extern osSemaphoreId BinarySem_TxUSB_HIDHandle;
extern osSemaphoreId BinarySem_TxUSB_CDCHandle;
extern sDixom Dixom;

#define dUsbState Dixom.Exchange.State.USB

const uint8_t CDC_Num = 0;


//----- USB CONNECTION STATUS TO HOST-------------------------------------------------------------------------------------------------//

void USB_Connected(){
	dUsbState.UsbConnection = Connected;
	Dixom.Basis.Settings.ParamNoSave[SettingsNoSave_VolUpUsbConnect_State].element = 1;
}

void USB_Connected_UsbHid(){
	Dixom.Exchange.State.USB.ConnectionHid = Connected;
	if(Dixom.Exchange.Interface.Interface_selection_mode == AUTO_SELECT){
		Dixom.Exchange.Interface.Current_interface = usbHid;
	}
}

void USB_Connected_UsbCdc(){
	Dixom.Exchange.State.USB.ConnectionCdc = Connected;
	if(Dixom.Exchange.Interface.Interface_selection_mode == AUTO_SELECT){
		Dixom.Exchange.Interface.Current_interface = USB_CDC;
	}
}

void USB_Disconnected(){
	dUsbState.UsbConnection = NotConnected;
	dUsbState.ConnectionHid = NotConnected;
	dUsbState.ConnectionCdc = NotConnected;
	Dixom.Exchange.State.USB.ConnectionHid = NotConnected,
	Dixom.Exchange.State.USB.ConnectionCdc = NotConnected,
	//GetCurrentExchangeInterface();
	Add_Packet_To_CircularBuffer_UsbStringRx((uint8_t *)"0 5 ", 4, allAvailable);
}

//----- STSTUS OF DATA DELIVERY TO THE HOST---------------------------------------------------------------------------------------------//

void USB_Sending_Completed(uint8_t EndPoint, uint8_t state){
	if(EndPoint == 1){
		osSemaphoreRelease(BinarySem_TxUSB_HIDHandle);

	}else if(EndPoint == 3){
		osSemaphoreRelease(BinarySem_TxUSB_CDCHandle);

	}
}

//----- DATA TRANSFER VIA USB VIRTUAL COM PORT (USB CDC)-----------------------------------------------------------------------------//

WorkRes Transmit_USB_CDC(uint8_t* Buf, uint16_t Len)
{
	if(Dixom.DevInfo.InMode != Initialization && dUsbState.ConnectionCdc == Connected){
		return ExchangeStatus( CDC_Transmit(CDC_Num, Buf, Len), USB_CDC_WRITE);
	}else{
		return ExchangeStatus(WR_NO_CONNECTION, USB_CDC_WRITE);
	}
}

//----- DATA TRANSFER VIA USB CUSTOM HID-----------------------------------------------------------------------------------------------//

uint8_t USB_HID_Transmit_FS(uint8_t *dataBuff){

	if(osSemaphoreWait(BinarySem_TxUSB_HIDHandle, 100) == osOK){
		//reset the send timer
	    Dixom.Exchange.Interface.TimeutTimerUsbHidSend = 0;
		return  ExchangeStatus(USBD_CUSTOM_HID_SendReport(&hUsbDevice, dataBuff, 65), USB_HID_WRITE);

	}else{
		ExchangeStatus(USB_HID_WRITE, WR_QUEUE_TIMEOUT);
		osSemaphoreRelease(BinarySem_TxUSB_HIDHandle);
		return WR_ERROR;
	}
}


uint8_t Transmit_Hid_Device(uint8_t *report, uint8_t size){
	return USBD_HID_Keybaord_SendReport(&hUsbDevice, report, size);
}








