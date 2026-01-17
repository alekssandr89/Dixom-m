
#include <Description.h>
#include <stdio.h>
#include <string.h>
#include "Exchange.h"
#include <Structures.h>
#include <DSP_Exchange.h>
#include <ExchangeConstant.h>
#include <base64.h>
#include <softCRC.h>
#include "cmsis_os.h"
#include <Bluetooth.h>
#include <Functions.h>

extern sDixom Dixom;


//----- EXCHANGE PERIPHERY INITIALIZATION-----------------------------------------------------------------------------------------------//


SemaphoreHandle_t mutex;
void Exchange_Initialization(){
	UART_Initialization();
	I2C_Initialization();
	mutex = xSemaphoreCreateMutex();

}

//----- PERIPHERAL EXCHANGE STATUS------------------------------------------------------------------------------------------------------//

#define dDetailLogs Dixom.Exchange.Logs.Detail

WorkRes ExchangeStatus(WorkRes State, uint8_t InterfaceCode){
	switch (State)
	{
	case HAL_OK            :  Write_ExchageLogs(InterfaceCode, WR_OK);              return  WR_OK;
	case HAL_ERROR         :  Write_ExchageLogs(InterfaceCode, WR_ERROR);           return  WR_ERROR;
	case HAL_BUSY          :  Write_ExchageLogs(InterfaceCode, WR_BUSY);            return  WR_BUSY;
	case HAL_TIMEOUT       :  Write_ExchageLogs(InterfaceCode, WR_TIMEOUT);         return  WR_TIMEOUT;
	case WR_QUEUE_TIMEOUT  :  Write_ExchageLogs(InterfaceCode, WR_QUEUE_TIMEOUT);   return  WR_QUEUE_TIMEOUT;
	case WR_NO_CONNECTION  :  Write_ExchageLogs(InterfaceCode, WR_NO_CONNECTION);   return  WR_NO_CONNECTION;
	default                :  Write_ExchageLogs(InterfaceCode, WR_OTHER);           return  WR_OTHER;
	}
}

typedef struct
{
	uint8_t     prefix[16];
	uint8_t     auxiliary[4];
	//uint8_t     msgEncoding;
	//uint8_t     auxiliary;
	uint8_t     data[MAX_OUT_MESSAGE_SIZE];
}sDataOut;
sDataOut dataOut;

struct
{
	uint8_t  encoding      :6;    //0-6 message encoding
	uint8_t  response      :1;    //7 response state: true, false
	uint8_t  repeated      :1;    //8 repeated message: true, false
} msg ;


uint8_t fControl[3];



void BluetoothTransmit(boolean byteData, uint16_t dataLen);
extern osSemaphoreId BinarySem_DataOutHandle;

boolean dataOutBlock = false;
boolean btDeliveryState = true;
/*
boolean isSendPermission(uint8_t interface, uint8_t manual, uint8_t intrefaceOut){
	if(((manual == ON || Dixom.Exchange.Interface.Current_interface == intrefaceOut)
					&& (intreface == intrefaceOut || (intrefaceOut == allAvailable || (Dixom.Exchange.Interface.Current_interface != intrefaceOut && intrefaceOut == allAvailableExceptCurrent))))
					|| intrefaceOut == allAvailableForced){
		return true;
	}
	return false;
}*/

//isSendPermission(usbHid, Dixom.Exchange.Interface.Manual_tx_usb_hid, intreface);


uint8_t TransmitDataOutputs(uint8_t* data, uint16_t dataLen,  boolean byteData,  boolean response, intreface intreface){

	if(dataOutBlock == false){
		dataOutBlock = true;
	}else{
		while(dataOutBlock == true){
			Delay(1, FreeRTOS);
		}
	}

	if(Dixom.DevInfo.InMode == Initialization){
		dataOutBlock = false;
		return WR_ERROR;
	}

	if (dataLen == AutoLenght) {
		dataLen = strlen((char*) data);
	}

	memcpy(dataOut.data, data, dataLen);

	uint16_t crc = ArcCrc16( dataOut.data, dataLen);
	fControl[1] = (crc >> 8) & 0xFF; // старший байт
	fControl[2] = crc & 0xFF; // младший байт


	if(dataLen>MAX_OUT_MESSAGE_SIZE-7 || dataLen<3){
		//-7 = "msgEncoding + crc16 + crc16 + base64 + data + space/r/n
		dataOutBlock = false;
		return WR_ERROR;
	}



	if( Dixom.Exchange.State.USB.ConnectionHid == Connected){

		if(((Dixom.Exchange.Interface.Manual_tx_usb_hid == ON || Dixom.Exchange.Interface.Current_interface == usbHid)
				&& (intreface == usbHid || (intreface == allAvailable || (Dixom.Exchange.Interface.Current_interface != usbHid && intreface == allAvailableExceptCurrent))))
				|| intreface == allAvailableForced){


			dataOut.auxiliary[1] = 2; //usb control byte

			if(dataLen < MAX_HID_MESSAGE_SIZE)	{
				 /*If the message is not more than 64 bytes, then send it as is*/
				 dataOut.auxiliary[2] = BYTE_WHOLE;
				 dataOut.auxiliary[3] = dataLen;
				 USB_HID_Transmit_FS((uint8_t *)&dataOut+17);
			 }else{
				 /*if the message is more than 64 bytes, then we split it into parts*/
				 dataOut.auxiliary[2] = BYTE_PART_START;
				 dataOut.auxiliary[3] = MAX_HID_MESSAGE_SIZE;
				 USB_HID_Transmit_FS((uint8_t *)&dataOut+17);
				 uint8_t SendMsgLen = 0;

				 for(uint8_t byte = MAX_HID_MESSAGE_SIZE; byte < dataLen; byte ++){
					 dataOut.data[SendMsgLen] = data[byte];
					 if (SendMsgLen == MAX_HID_MESSAGE_SIZE){
						 if(dataLen == byte){
							 dataOut.auxiliary[2] = BYTE_PART_END;
						 }else{
							 dataOut.auxiliary[2] = BYTE_PART;
						 }
						 dataOut.auxiliary[3] = SendMsgLen;
						 USB_HID_Transmit_FS((uint8_t *)&dataOut+17);
						 SendMsgLen = 0;
					 }
					 SendMsgLen++;
				 }

				 if(SendMsgLen !=0){
					 dataOut.auxiliary[2] = BYTE_PART_END;
					 dataOut.auxiliary[3] = SendMsgLen;
					 USB_HID_Transmit_FS((uint8_t *)&dataOut+17);
				 }
			 }
		}
	}

	//------------experimental version---------------------//





	if(byteData == true){
		uint32_t bufSize = MAX_OUT_MESSAGE_SIZE-6;
		if(base64_encode( dataOut.data, &bufSize, data, dataLen ) == POLARSSL_ERR_BASE64_BUFFER_TOO_SMALL){
			dataOutBlock = false;
			return WR_ERROR;
		}
		dataLen = bufSize;
	    msg.encoding = BASE64_WHOLE;
	}else{
		msg.encoding = UTF8_WHOLE;
	}


	dataOut.data[dataLen] = '\r';
	dataOut.data[dataLen+1] = '\n';
	dataLen = dataLen+6;








	if(Dixom.Module.Bluetooth.Connection == Connected){

		if(((Dixom.Exchange.Interface.Manual_tx_bluetooth == ON || Dixom.Exchange.Interface.Current_interface == BLUETOOTH_SPP)
				&& (intreface == BLUETOOTH_SPP || (intreface == allAvailable || (Dixom.Exchange.Interface.Current_interface != BLUETOOTH_SPP && intreface == allAvailableExceptCurrent))))
				|| intreface == allAvailableForced){

			if(Dixom.Module.Bluetooth.Settings[BtDeliveryGuarantee].Data == ON){
				msg.response = response;
			}else{
				msg.response = false;
			}

			msg.repeated = false;
			fControl[0] = *((uint8_t*)&msg);

			BluetoothTransmit(byteData, dataLen);

			if(msg.response == true){
				// Delivery Guarantee within 4 sec or disable the interface
				uint16_t timeout = 0;
				int8_t attempts = 20;

				while((btDeliveryState == unknown || btDeliveryState == false) && attempts){
					timeout++;
					if(timeout > 200 || btDeliveryState == false){
						timeout = 0;
						msg.repeated = true;
						fControl[0] = *((uint8_t*)&msg);
						BluetoothTransmit(byteData, dataLen);
						attempts--;
					}
					Delay(1, FreeRTOS);
				}

				if(!attempts){
					//disable interface
					timeout = 0;
				}
			}
		}
	}

	if(Dixom.Exchange.State.USB.ConnectionCdc == Connected){
		if(((Dixom.Exchange.Interface.Manual_tx_usb_cdc == ON || Dixom.Exchange.Interface.Current_interface == USB_CDC)
				&& (intreface == USB_CDC ||  (intreface == allAvailable || (Dixom.Exchange.Interface.Current_interface != USB_CDC && intreface == allAvailableExceptCurrent))))
				|| intreface == allAvailableForced){
			/*
				if(BinarySem_TxUSB_CDCHandle != NULL){
					if(osSemaphoreWait(BinarySem_TxUSB_CDCHandle, 200) == osOK){
						ExchangeStatus(CDC_Transmit(CDC_Num, (uint8_t *)&dataOut+16, dataLen), USB_CDC_WRITE);
					}else{
						ExchangeStatus(USB_CDC_WRITE, WR_QUEUE_TIMEOUT);
						osSemaphoreRelease(BinarySem_TxUSB_CDCHandle);
					}
				}
			}*/
		}
	}


	dataOutBlock = false;
	return WR_OK;
}

void BluetoothTransmit(boolean byteData, uint16_t dataLen){
	if( Dixom.Module.Bluetooth.Connection == Connected){
	    uint8_t vreme[5];
		uint32_t buffSize = 5;
		if(Dixom.Module.Bluetooth.SppThroughputMode == ON){
			//Fast mode
			base64_encode( vreme, &buffSize,  fControl, 3);
			for(uint8_t i = 0; i< 4; i++){
				dataOut.auxiliary[i] = vreme[i];
			}
			UART1_Transmit((uint8_t *)&dataOut+16, dataLen, 0, FreeRTOS);

		}else{

			uint8_t AddDataLen = 13; //AT+SPPSEND=%hu,
			uint8_t prefixBias = 3;  //%hu 1-9 || 10-99 || 100-999

			if(dataLen>99){
				AddDataLen = 15 ;
				prefixBias = 1;
			}else if(dataLen>9) {
				AddDataLen = 14;
				prefixBias = 2;
			}
			sprintf((char *) dataOut.prefix+prefixBias, "AT+SPPSEND=%hu,", dataLen);

			if(byteData == true){
				msg.encoding = BASE64_WHOLE;
			}else{
				msg.encoding = UTF8_WHOLE;
			}

			//If Sp Throughput Mode is set to OFF, it uses the delivery mechanism built into the module
			uint8_t ildrsponseState = msg.response;
			msg.response = false;
			fControl[0] = *((uint8_t*)&msg);
			msg.response = ildrsponseState;

			base64_encode( vreme, &buffSize,  fControl, 3);
			for(uint8_t i = 0; i< 4; i++){
				dataOut.auxiliary[i] = vreme[i];
			}

			uint8_t deliveryTimeOut = 0;

			while(btDeliveryState != true){
				Delay(1, FreeRTOS);
				if(deliveryTimeOut > 25){
					btDeliveryState = true;
				}else{
					deliveryTimeOut++;
				}
			}
			UART1_Transmit((uint8_t *)&dataOut+prefixBias, dataLen+AddDataLen, 0, FreeRTOS);
		}
	}
}






//----- PERIPHERAL EXCHANGE LOG---------------------------------------------------------------------------------------------------------//

void Write_ExchageLogs(uint8_t InterfaceCode, uint8_t ExchageState){

 	if(dDetailLogs.Interface[InterfaceCode].Code == EMPTY){
 		dDetailLogs.Interface[InterfaceCode].Code = InterfaceCode;
 		dDetailLogs.InterfacesInAction++;
 	}

 	if(ExchageState == WR_OK){
 		dDetailLogs.Count_Total_WR_OK++;
 	 	dDetailLogs.Interface[InterfaceCode].Count_WR_OK++;
 	}else if(ExchageState == WR_ERROR){
 		dDetailLogs.PresenceOfErrors = YES;
 		dDetailLogs.Count_Total_WR_ERROR++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_ERROR++;
 	}else if(ExchageState == WR_BUSY){
 		dDetailLogs.Count_Total_WR_BUSY++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_BUSY++;
 	}else if(ExchageState == WR_TIMEOUT){
 		dDetailLogs.Count_Total_WR_TIMEOUT++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_TIMEOUT++;
 	}else if(ExchageState == WR_QUEUE_TIMEOUT){
 		dDetailLogs.Count_Total_WR_QUEUE_TIMEOUT++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_QUEUE_TIMEOUT++;
 	}else if(ExchageState == WR_NO_CONNECTION){
 		dDetailLogs.Count_Total_WR_NO_CONNECTION++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_NO_CONNECTION++;
 	}else if(ExchageState == WR_OTHER){
 		dDetailLogs.Count_Total_WR_OTHER++;
 		dDetailLogs.Interface[InterfaceCode].Count_WR_OTHER++;
 	}
}


//----- PARAMETRIC ARRAY COPYING-------------------------------------------------------------------------------------------------------//

void Copy_ArrayToArray(uint8_t from[], uint16_t fromBias, uint8_t to[],  uint16_t toBias, uint16_t count, uint16_t toMaxBuff){
	if(count == AutoLenght){
		count = strlen((char*)from);
	}
	for(uint8_t byte = 0; byte<count; byte ++ ){
		if(toMaxBuff == 0){
			to[byte + toBias] = from[byte + fromBias];
		}else if(byte + toBias < toMaxBuff){
			to[byte + toBias] = from[byte + fromBias];
		}
	}
}

void Copy_Array(uint8_t *from[], uint16_t fromBias, uint8_t *to[],  uint16_t toBias, uint16_t count, uint16_t toMaxBuff){
	if(count == AutoLenght){
		count = strlen((char*)from);
	}
	for(uint8_t byte = 0; byte<count; byte ++ ){
		if(toMaxBuff == 0){
			to[byte + toBias] = from[byte + fromBias];
		}else if(byte + toBias < toMaxBuff){
			to[byte + toBias] = from[byte + fromBias];
		}
	}
}

//----- VARIBLE TYPE CONVERTER-------------------------------------------------------------------------------------------------------//

uint32_t Converter_8_to_u32(uint8_t* bytes) {
  uint32_t u32 = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
  return u32;
}

void Converter_u32_to_u8 (uint32_t u32, uint8_t* u8) {
  u8[0] = (u32 & 0xff000000) >> 24;
  u8[1] = (u32 & 0x00ff0000) >> 16;
  u8[2] = (u32 & 0x0000ff00) >> 8;
  u8[3] = u32 & 0x000000ff;
}



void GATEWAY(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String){

	size_t dataSize = strlen(Received_String);

	uint8_t txBuf[dataSize+2];
	uint8_t MessageEndLine[2] = {'\r','\n'};
	Copy_ArrayToArray(&Received_String[0], 0,  &txBuf[0],  0, dataSize, dataSize+2);
	Copy_ArrayToArray(&MessageEndLine[0], 0,  &txBuf[0],  dataSize, 2, dataSize+2);


	if(Function == usbHid)      { USB_HID_Transmit_FS(txBuf);	}
	if(Function == USB_CDC)      { CDC_Transmit(0, txBuf, dataSize);	                }
	if(Function == BLUETOOTH_SPP){ UART1_Transmit(txBuf, dataSize, 0, FreeRTOS);	    }
	if(Function == ARDUINO)      {  	}
	if(Function == NEXTION)      {  	}
	if(Function == RF_MODULE)    {   	}
}


//----- READING AND WRITING CHIP REGISTERS-----------------------------------------------------------------------------------//


int16_t  SelectDevice = 0;
uint32_t RegAddr = 0;
uint32_t RegValue = 0;
uint8_t  RegValueByte[4];
uint8_t I2C_Read_Block = 0;

void BIT_CHIP_CONTROL(uint8_t ControlByte, uint8_t *Recive_USB){

	SelectDevice = 0;
	RegAddr = 0;
	RegValue = 0;

	sscanf((char *) Recive_USB, "%hd %lu %lu",&SelectDevice, &RegAddr, &RegValue);

    if(ControlByte == DATA_SET){ //SET
    	RegValueByte[0]  = RegValue;
    	RegValueByte[1]  = RegValue >> 8;
    	RegValueByte[2]  = RegValue >> 16;
    	RegValueByte[3]  = RegValue >> 24;

    	if(SelectDevice == ByteControl_Dev_AK4440){

    	}else if(SelectDevice == ByteControl_Dev_ADAU1962){

    	}else if(SelectDevice == ByteControl_Dev_AK4458){
    		if (DevState_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, 1, 100) != WR_OK) {
    			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);
    		}else{
    			Transmit_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
    		}
    	}else if(SelectDevice == ByteControl_Dev_PCM1690){
    		if (DevState_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

    		}else{
    			Transmit_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
    		}
    	}
    	else if(SelectDevice == ByteControl_Dev_RDA5807){

    	}else if(SelectDevice == ByteControl_Dev_DS3231){
    		if (DevState_I2C2(DEVICE_ADDR_RTC_DS3231, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

    		}else{
    			Transmit_I2C2(DEVICE_ADDR_RTC_DS3231, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
    		}
    	}else if(SelectDevice == ByteControl_Dev_TAS3251){

    	}else if(SelectDevice == ByteControl_Dev_SI4735){
    		if (DevState_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, 1, 100) != WR_OK) {
    			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);
    		}else{
    			Transmit_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
    		}
    	}else if(SelectDevice == ByteControl_Dev_ADAU1452){

    		Transmit_DSP(RegAddr, &RegValueByte[0], 4, 1500);

    	}else if(SelectDevice == ByteControl_Dev_TDF8551J){
    		if (DevState_I2C2(DEVICE_ADDR_AMP_TDF8551J<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

    		}else{
    			Transmit_I2C2(DEVICE_ADDR_AMP_TDF8551J<<1, RegAddr, 1, &RegValueByte[0], 4, 200, ExchangeMemMainSteam);
    		}
    	}else{
 			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_2, AutoLenght,  false,  true, allAvailable);

    		return;
    	}
 		TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_1, AutoLenght,  false,  true, allAvailable);

    	RegValue= 0;
    	RegValueByte[0] = 0;
    	RegValueByte[1] = 0;
    	RegValueByte[2] = 0;
    	RegValueByte[3] = 0;

    }else if(ControlByte == DATA_GET){ //GET
    	I2C_Read_Block = 1;
    }else{
    	TransmitDataOutputs((uint8_t *)GeneralNotifi_CtlBitNotSupported, AutoLenght,  false,  true, allAvailable);
    }
}

void Regiser_Read_Loop(){

	if(I2C_Read_Block == 1){

		I2C_Read_Block = 0;
		uint8_t SendReady = NO;

		if(SelectDevice == ByteControl_Dev_AK4440){
			if (DevState_I2C2(DEVICE_ADDR_DAC_AK4440<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{
				Receiver_I2C2(DEVICE_ADDR_DAC_AK4440<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMainSteam);
				SendReady = YES;
			}
		}else if(SelectDevice == ByteControl_Dev_ADAU1962){
			if (DevState_I2C2(DEVICE_ADDR_DAC_ADAU1962, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{

			}
		}else if(SelectDevice == ByteControl_Dev_AK4458){
			if (DevState_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{
				Receiver_I2C2(DEVICE_ADDR_DAC_AK4458VN<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}
		}else if(SelectDevice == ByteControl_Dev_PCM1690){
			if (DevState_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{
				Receiver_I2C2(DEVICE_ADDR_DAC_PCM1690<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}
		}else if(SelectDevice == ByteControl_Dev_RDA5807){
			if (DevState_I2C1(DEVICE_ADDR_RADIO_RDA5807, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);
			}else{

			}
		}else if(SelectDevice == ByteControl_Dev_DS3231){
			if (DevState_I2C2(DEVICE_ADDR_RTC_DS3231, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);
			}else{
				Receiver_I2C2(DEVICE_ADDR_RTC_DS3231, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}
		}else if(SelectDevice == ByteControl_Dev_TAS3251){

			if (DevState_I2C2(DEVICE_ADDR_DAC_TAS3251, 1, 100) != WR_OK) {
				/* Return false */
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{
			/*	Transmit_I2C2(DEVICE_ADDR_DAC_TAS3251<<1, NOT_USED, NOT_USED, &addr, 1, 200, ExchangeMainSteam);
				Receiver_I2C2(DEVICE_ADDR_DAC_TAS3251<<1, NOT_USED, NOT_USED, &RegValueByte, 1, 200, ExchangeMainSteam);
				sprintf( (char *)Dixom.Buff,"BIT_ARRAY: %hhu %hhu",addr, READ);
				Transmit_Data_Out(&Dixom.Buff[0], AutoLenght, 0, FreeRTOS, NO);*/
			}

		}else if(SelectDevice == ByteControl_Dev_SI4735){

			if (DevState_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{

				Receiver_I2C1(DEVICE_ADDR_RADIO_SI4735<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}

		}else if(SelectDevice == ByteControl_Dev_24LC256){

			if (DevState_I2C2(DEVICE_ADDR_EEPROM_24LC256<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{

				Receiver_I2C2(DEVICE_ADDR_EEPROM_24LC256<<1, RegAddr, 1, &RegValueByte[0], 1, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}

		}else if(SelectDevice == ByteControl_Dev_TDF8551J){

			if (DevState_I2C2(DEVICE_ADDR_AMP_TDF8551J<<1, 1, 100) != WR_OK) {
     			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_3, AutoLenght,  false,  true, allAvailable);

			}else{

				Receiver_I2C2(DEVICE_ADDR_AMP_TDF8551J<<1, RegAddr, 1, &RegValueByte[0], 3, 200, ExchangeMemMainSteam);
				SendReady = YES;
			}

		}else if(SelectDevice == ByteControl_Dev_ADAU1452){

			Receiver_DSP( RegAddr,    &RegValueByte[0], 4, 500);
			SendReady = YES;

		}else{
 			TransmitDataOutputs((uint8_t *)Basis_bitControl_1_6_2, AutoLenght,  false,  true, allAvailable);

			return;
		}


		if(SendReady == YES){

			RegValue  =  RegValueByte[0]  |(RegValueByte[1] << 8) |(RegValueByte[2] << 16) |(RegValueByte[3] << 24);

			sprintf( (char *)Dixom.Buff,Basis_bitControl_1_6_0,RegAddr, RegValue);
  			TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);


			RegValue= 0;
			RegValueByte[0] = 0;
			RegValueByte[1] = 0;
			RegValueByte[2] = 0;
			RegValueByte[3] = 0;
		}
	}
}








