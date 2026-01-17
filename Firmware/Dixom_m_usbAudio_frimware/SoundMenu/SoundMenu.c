

#include "SoundMenu.h"
#include "STM32_Register.h"
#include <string.h>

volatile sI2C_TypeDef ResetFlag;

sUsbAudio                UsbAudio;

uint8_t                  I2C1_Rx_Buff[255];
uint8_t                  I2C1_Tx_Buff[255];


void Init_Sound_Param(){

	UsbAudio.State.InMode = ReadyForWork;
	UsbAudio.State.AudioConfig = Stereo_2_0;
	UsbAudio.State.BitRate = BIT_24;
	UsbAudio.State.BusType = I2S;
	UsbAudio.State.Mute = 0;
	UsbAudio.State.Playback = 0;
	UsbAudio.State.SamplingFreq = SFreq_48000;
	UsbAudio.State.USB = NotConnected;
	UsbAudio.State.Volume = 1;


	UsbAudio.Device.Version_Main = 1005;      //1.005
	UsbAudio.Device.Version_Bootloader = 100; //1.00
	UsbAudio.Device.Model = Dixom_HiRes_Audio;
	UsbAudio.Device.Pcb_Type = Integrated;
	UsbAudio.Device.Circuit_Revision = 100;   //1.00
	UsbAudio.Device.External_PcbRevision = 0;
}

void OldRestart();

void SoundMenu(){

	OldRestart();

	if(I2C1_Rx_Buff[0] == ADDR_REG_RESTART)          {  NVIC_SystemReset();                                                               }
	if(I2C1_Rx_Buff[0] == ADDR_REG_USB_AUDIO)        {  memcpy(&I2C1_Tx_Buff[0], (uint8_t*)&UsbAudio, sizeof(UsbAudio));                  }
	if(I2C1_Rx_Buff[0] == ADDR_REG_USB_AUDIO_STATE)  {  memcpy(&I2C1_Tx_Buff[0], (uint8_t*)&UsbAudio.State, sizeof(UsbAudio.State));      }
	if(I2C1_Rx_Buff[0] == ADDR_REG_USB_AUDIO_DEVICE) {  memcpy(&I2C1_Tx_Buff[0], (uint8_t*)&UsbAudio.Device, sizeof(UsbAudio.Device));    }
}


void OldRestart(){
	if(I2C1_Rx_Buff[0] == 'R' && I2C1_Rx_Buff[1] == 'E' && I2C1_Rx_Buff[2] == 'S'){
		NVIC_SystemReset();
	}
}


/*
 * Так как мы работаем в режиме Slave нам не нужно самим отправлять данные, их заберёт мастер при необходимости.
 * Но нам нужно позаботиться от данных которые он заберет. Данные которые он заберёт находятся в буфере I2C1_Tx_Buffх[0]
 * поэтому, необходимо позвботится о его наполении во время выхова функции I2C_1_Slave_Rx_Cmplt в режиме Write. Также в режиме Write
 * нет смысла отправлять длину передаваемых данных, так как мастер сам решает сколько ему забрать. В режиме Read возвращается Data_Lenght
 * которая указывает сколько байт данных прислал Мастер слейву.
 *
 */
void I2C_1_Slave_Exchange(uint8_t Direction, uint8_t Data_Lenght){

	if(Direction == Write){
		SoundMenu();
	}else{
		SoundMenu();
	}
}

/* Обратобка ошибок, возникает при:
 *
 * This interrupt is generated when:
 * – BERR = 1
 * – ARLO = 1
 * – AF = 1
 * – OVR = 1
 * – PECERR = 1
 * – TIMEOUT = 1
 * – SMBALERT = 1
 */
void I2C_1_ER_IRQHandler(){

	//sI2C1->SR1.OVR = 0;
}

/* Обратобка событий, возникает при:
 *
 * This interrupt is generated when:
 * – SB = 1 (Master)
 * – ADDR = 1 (Master/Slave)
 * – ADD10= 1 (Master)
 * – STOPF = 1 (Slave)
 * – BTF = 1 with no TxE or RxNE event
 * – TxE event to 1 if ITBUFEN = 1
 * – RxNE event to 1if ITBUFEN = 1
 */
void I2C_1_EV_IRQHandler(){

	static uint8_t Data_Lenght;
	ResetFlag.SR1 = sI2C1->SR1;

	//MASTER -> SET SLAVE ADDRESS
	if(sI2C1->CR2.ITEVTEN == 1 && sI2C1->SR1.ADDR == 1){

		//RESET DATA LENGHT
		Data_Lenght = 0;

		//RESET ADDR
		while(sI2C1->SR1.ADDR == 1){
			ResetFlag.SR2 = sI2C1->SR2;
		}

		//MASTER -> SLAVE FIRST BYTE (If write direction)
		if(sI2C1->SR1.TxE == 1 && sI2C1->SR2.TRA == 1){
			I2C_1_Slave_Exchange(Write, Data_Lenght);
			while(sI2C1->SR1.TxE == 1){
				sI2C1->DR.DR = I2C1_Tx_Buff[Data_Lenght];
				Data_Lenght++;
			}
		}

	//MASTER -> SLAVE NEXT BYTE
	}else if(sI2C1->SR2.TRA == 1 && sI2C1->CR2.ITEVTEN ==1  && sI2C1->SR1.BTF == 1){
		while(sI2C1->SR1.BTF == 1){
			sI2C1->DR.DR = I2C1_Tx_Buff[Data_Lenght];
			Data_Lenght++;
		}


	//SLAVE -> MASTER FIRST AND NEXT BYTE
	}else if(sI2C1->SR2.TRA == 0 && sI2C1->SR1.RxNE == 1 && sI2C1->CR2.ITBUFEN ==1  && sI2C1->SR1.BTF == 0){
		while (sI2C1->SR1.RxNE == 1){
			I2C1_Rx_Buff[Data_Lenght] = sI2C1->DR.DR;
			Data_Lenght++;
		}


	//SLAVE -> MASTER NEXT BYTE
	}else if(sI2C1->SR2.TRA == 0 && sI2C1->CR2.ITEVTEN ==1  && sI2C1->SR1.BTF == 1){
		while (sI2C1->SR1.BTF == 1){
			I2C1_Rx_Buff[Data_Lenght] = sI2C1->DR.DR;
			Data_Lenght++;
		}


    //STOP
	}else if(sI2C1->CR2.ITEVTEN == 1 && sI2C1->SR1.STOPF == 1){
		sI2C1->CR2.ITERREN = 0;
		sI2C1->CR2.ITEVTEN = 0;
		sI2C1->CR2.ITBUFEN = 0;
		while(sI2C1->SR1.STOPF == 1){
			ResetFlag.SR1 = sI2C1->SR1;
			sI2C1->CR1.ACK = 0;
			sI2C1->CR1.PE = 1;
		}
		if(Data_Lenght!=0){
			I2C_1_Slave_Exchange(Read, Data_Lenght);
		}
	}

	// RESET NACK
	if(sI2C1->SR1.AF == 1){
		sI2C1->SR1.AF= 0;
	}

	//ENABLE INTERUPT
	I2C_1_START_IRQHandler();
}


//ENABLE INTERUPT
void I2C_1_START_IRQHandler(){

	sI2C1->CR1.ACK = 1;      //разрешаем отправлять ACK/NACK после приема байта адреса или данных.
//	sI2C1->CR2.ITERREN = 1;  //разрешаем прерывание при возникновении ошибок.
	sI2C1->CR2.ITBUFEN = 1;
	sI2C1->CR2.ITEVTEN = 1;  //разрешаем преывание по событию
}
