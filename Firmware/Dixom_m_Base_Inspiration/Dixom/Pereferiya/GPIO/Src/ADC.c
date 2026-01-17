#include <stdint.h>
#include <string.h>
#include <stm32f4xx_hal.h>
#include "Encoder.h"
#include "GPIO.h"
#include "ADC.h"
#include "stdio.h"
#include "Exchange.h"
#include <Math.h>
#include "Analog.h"
#include <Structures.h>

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

extern sDixom Dixom;


#define  dAnalog        Dixom.Control.Analog


void ADC_Start_DMA(){
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*) &Dixom.Service.Acc_Light_ADC_Val,2);
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*) &Dixom.Service.Btn_ADC_Val,4);
}

void Action_Analog_Control_Loop(){

	Analog_ActionBtn();

	Dixom.Service.ACC.Voltage = (0.0645* Dixom.Service.Acc_Light_ADC_Val[0]  +0.41);

	for (short channel = 0; channel < 4; channel++) {
		if ((Dixom.Service.Btn_ADC_Val[channel]<3900) && (dAnalog.CH[channel].BlockStatus==0) ){
			dAnalog.CH[channel].ResButBlockTimer=0;
			dAnalog.CH[channel].BlockStatus=1;
			ChEvent(channel);
		}else if(Dixom.Service.Btn_ADC_Val[channel]>3900){
			dAnalog.CH[channel].ResButBlockTimer=0;
			dAnalog.CH[channel].BlockStatus = 0;
			if(dAnalog.CH[channel].ValueAction  != 4444){
				Analog_ActionCH(channel+1, dAnalog.CH[channel].ValueAction, NO);
				dAnalog.CH[channel].ValueAction = 4444;
			}
			Analog_ActionCH(channel+1, 4444, NO);
		}else{
			dAnalog.CH[channel].ResButBlockTimer++;
			dAnalog.CH[channel+1].HoldStatus = 1;
			if(dAnalog.CH[channel].ResButBlockTimer>5000){
				dAnalog.CH[channel].ResButBlockTimer=0;
				sprintf( (char*)Dixom.Buff,"BLOCK RESBUT CH %hu =%hu>", channel+1, Dixom.Service.Btn_ADC_Val[channel] );
  		  	    TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);

			}
		}
	}
}

void  ChEvent (short CH){

	for (int i=0; i<dAnalog.ZamerCount; i++){
		if(dAnalog.ADC_DataCH >  Dixom.Service.Btn_ADC_Val[CH]){
			dAnalog.ADC_DataCH=Dixom.Service.Btn_ADC_Val[CH];
		}
	}
	dAnalog.CH[CH+1].HoldStatus = 1;
	dAnalog.CH[CH].ValueAction = dAnalog.ADC_DataCH;
	Analog_ActionCH(CH+1, dAnalog.ADC_DataCH, YES);
	dAnalog.ADC_DataCH=4444;
}

void ADC_READ(void){
	sprintf((char*)Dixom.Buff,"�H1-%d �H2-%d ENC1-%d ENC2-%d",Dixom.Service.Btn_ADC_Val[0],Dixom.Service.Btn_ADC_Val[1],Dixom.Service.Btn_ADC_Val[2],Dixom.Service.Btn_ADC_Val[3]);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);


}

void ACC_READ(){
	sprintf((char*)Dixom.Buff,"%hu  ",Dixom.Service.Acc_Light_ADC_Val[0]);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}

void LIGHT_READ(){
	sprintf((char*)Dixom.Buff,"%hu  ",Dixom.Service.Acc_Light_ADC_Val[Light]);
	TransmitDataOutputs(Dixom.Buff, AutoLenght,  false,  true, allAvailable);
}








