#include <Structures.h>
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "Encoder.h"
#include <Control_Constant.h>

extern sDixom Dixom;

#define  dEncoder        Dixom.Control.Analog.Encoder

void ENCODER1(void){

	dEncoder.enc1Data=TIM3->CNT;

	if(dEncoder.enc1Data!=dEncoder.enc1Data2){
		dEncoder.Direction_ENC1= TIM3->CR1;

		if(Dixom.DevInfo.InMode != FirmwareUpdate){
			if(Dixom.Control.Settings[Encoder1Type].param == 0){
				ecnDirection1();
			}else{
				dEncoder.duoEnc1++;
				if(dEncoder.duoEnc1 >1){
					dEncoder.duoEnc1 = 0;
					ecnDirection1();
				}
			}
		}
	}
	dEncoder.enc1Data2=TIM3->CNT;
}

void ENCODER2(void){

	dEncoder.enc2Data=TIM8->CNT;
    if(dEncoder.enc2Data!=dEncoder.enc2Data2){
    	dEncoder.Direction_ENC2= TIM8->CR1;

    	if(Dixom.DevInfo.InMode != FirmwareUpdate){
    		if(Dixom.Control.Settings[Encoder2Type].param == 0){
    			ecnDirection2();
    		}else{
    			dEncoder.duoEnc2++;
    			if(dEncoder.duoEnc2 >1){
    				dEncoder.duoEnc2 = 0;
    				ecnDirection2();
    			}
    		}
    	}
    }
    dEncoder.enc2Data2=TIM8->CNT;
}


// 100 = ENC 1 LEFT
// 200 = ENC 1 RIGHT
// 300 = ENC 2 LEFT
// 400 = ENC 2 RIGHT


////////////////////////////////////ENCODER 1///////////////////////////////////
void ecnDirection1(void){

	if(dEncoder.Direction_ENC1==1){//Direction LEFT
		Analog_ActionCH(0, 1000, 1);
	//	Delay(5, 1);
		Analog_ActionCH(0, 1000, 0);

	}else{                //Direction RIGHT
		Analog_ActionCH(0, 500, 1);
	//	Delay(5, 1);
		Analog_ActionCH(0, 500, 0);
	}
	Analog_ActionCH(0, 4444, 0);
}



////////////////////////////////////ENCODER 2///////////////////////////////////
void ecnDirection2(void){

	if(dEncoder.Direction_ENC2==1){//Direction LEFT
		Analog_ActionCH(0, 2000, 1);
	//	Delay(5, 1);
		Analog_ActionCH(0, 2000, 0);
	}else{                //Direction RIGHT
		Analog_ActionCH(0, 1500, 1);
	//	Delay(5, 1);
		Analog_ActionCH(0, 1500, 0);
	}
	Analog_ActionCH(0, 4444, 0);
}























