/*
 * AnalogControl.h
 *
 *  Created on: 7 ����. 2021 �.
 *      Author: Dixom
 *      Project: DSP Audio DIXOM-m
 */
#ifndef MODULE_ANALOGCONTROL_ANALOGCONTROL_H_
#define MODULE_ANALOGCONTROL_ANALOGCONTROL_H_

#include <Analog_Constant.h>
#include "stdint.h"
void       SET_EXTERNAL_CONTROL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);
WorkRes    AnalogControl_Initialization_Final();
WorkRes    AnalogControl_Struct_Default(uint8_t OnlyEditingReset);
void       setDefaultControlSettings();
void       CONTROL_Show_Settings(uint8_t ShowType);
void       CONTROL_Settings(uint8_t ControlByte, uint8_t *Received_String);
void       ANALOG_SET_BUTTON(uint8_t ControlByte, uint8_t*  Recive_USB);     // EXAMPLE:    SET BTN 6 1000 30 10 100 10 100 100 10 100 10 0  0  0  0
                                //                                             1   2  3   4  5  6   7   8  9  10  11 12 13 14 15
void        SetResButton(short channel,                 // 1-------------------#---|--|---|--|--|---|---|--|--|---|--|--|--|--|-----1  ����� ������
                         short adcValue,                // 2-----------------------#--|---|--|--|---|---|--|--|---|--|--|--|--|-----2  �������� ��������� ��� (�������� ������)
                         uint8_t interval,              // 3--------------------------#---|--|--|---|---|--|--|---|--|--|--|--|-----3  ����������� ��������� ���
                         short action,                  // 4------------------------------#--|--|---|---|--|--|---|--|--|--|--|-----4  ���������� �������� 1
                         short delayAction,             // 5---------------------------------#--|---|---|--|--|---|--|--|--|--|-----5  ���������� �������� ��� ����������� 1
                         short doublAction,             // 6------------------------------------#---|---|--|--|---|--|--|--|--|-----6  ���������� �������� ��� ������� ����� 1
                         short delayTiming,             // 7----------------------------------------#---|--|--|---|--|--|--|--|-----7  ������ ��� ���������
                         short doublTiming,             // 8--------------------------------------------#--|--|---|--|--|--|--|-----8  ������ �������� �����
                         short action2,                 // 9-----------------------------------------------#--|---|--|--|--|--|-----9  ���������� �������� 2
                         short delayAction2,            // 10-------------------------------------------------#---|--|--|--|--|-----10 ���������� �������� ��� ����������� 2
                         short doublAction2,            // 11-----------------------------------------------------#--|--|--|--|-----11 ���������� �������� ��� ������� ����� 2
                         short param1,                  // 12--------------------------------------------------------#--|--|--|-----12 �������� 1
                         short param2,                  // 13-----------------------------------------------------------#--|--|-----13 �������� 2
                         short param3,                  // 14--------------------------------------------------------------#--|-----14 �������� 3
                         short param4);                 // 15-----------------------------------------------------------------#-----15 �������� 4

 void       ANALOG_GET_BUTTON(uint8_t*  Recive_USB);    // EXAMPLE: GET RESBUT 0 1
                                                        //                     1 2
 void        Analog_GetButton(short cnannel,            // 1-------------------#-|--------------------------------------------------1  ����� ������
                              short point,              // 2---------------------#--------------------------------------------------2  ����� ����� � ������ (�������� 15 ����� ������� 0)
                              uint8_t AllOrModified);

 void       ANALOG_GET_ALL_BUTTON(uint8_t *Recive_USB);
 void       SHOW_CONTROL_PARAMETERS(uint8_t AllOrModified);
 void       ANALOG_BUTTON_SHOW_PARAMETERS(uint8_t AllOrModified);
 void       Analog_LogicOfActions();
 void       Analog_ActionBtn(void);
 void       Analog_ResetCH(uint8_t channel);
 void       Analog_ChannelBlock(short channel);
 void       Analog_ActionCH(short channel, short value, short pressed);
 void       Analog_SetButton(short channel, short adcValue, uint8_t interval, short action, short delayAction, short doublAction, short delayTiming, short doublTiming,
                             short action2, short delayAction2, short doublAction2, short param1, short param2, short param3, short param4);
 void       ANALOG_SET_BUTTON_NAME(uint8_t ControlByte, uint8_t *Recive_USB);    // EXAMPLE: SET NAME BTN 1 100 NAME
 //                                                                                  1  2   3
 //                                                                                  #--|---|---------- ����� ������ � ������ (8 ���)
 //                                                                                     #---|---------- �������� � ������ (32����)
 //                                                                                         #---------- ��� ������
 void       Analog_AppInitBTN(void);

typedef struct{
	uint16_t    adcValue;               //2 bytes   //�������� ������
	uint16_t    action;                 //2 bytes   //�������� �� 1 ����
	uint16_t    action2;                //2 bytes   //�������� 2 �� 1 ����
	uint16_t    delayAction;            //2 bytes   //�������� ��� ���������
	uint16_t    delayAction2;           //2 bytes   //�������� 2 ��� ���������
	uint16_t    doublAction;            //2 bytes   //�������� �� ������� ����
	uint16_t    doublAction2;           //2 bytes   //�������� 2 �� ������� ����
	uint8_t     interval;               //1 bytes   //�����������
	uint8_t     editing;                //1 bytes   //���������� �� ������
	uint16_t    delayTiming;            //2 bytes   //�������� ���������
	uint16_t    doublTiming;            //2 bytes   //�������� �������� �����
	uint16_t    param1;                 //2 bytes   //�������� 1
	uint16_t    param2;                 //2 bytes   //�������� 2
	uint16_t    param3;                 //2 bytes   //�������� 3
	uint16_t    param4;                 //2 bytes   //�������� 4
	uint8_t     BtnName[BtnNameLenght]; //30 bytes  //�������� ������
} sAnalogControl; //������ 28 + 36 = 64 ����

typedef struct
{
	sAnalogControl  Button[NumberButtons];
	uint16_t        ActionTimer;
	uint16_t        ChTimer;
	uint16_t        CHN_ADDR;
	uint8_t         ActionStatus;
	uint8_t         HoldStatus;
	uint8_t         Point;
	uint8_t         UpdIonterval;
	uint8_t         ActionRelease;
	uint8_t         doublStatus;
	uint8_t         pressed;
	uint8_t         ChClick;
	uint8_t         ActionComplite;
	uint8_t         ActionPreset;
	uint16_t        ValueAction;
	uint16_t        ResButBlockTimer;
	uint8_t         BlockStatus;
} sResButtonCh;

typedef struct
{
	uint8_t               timer;
	uint8_t               direction;
} sEncoderSettings;

typedef struct
{
	uint8_t               timer;
} sResButtonSettings;

typedef struct
{
	uint32_t     enc1Data;
	uint32_t     enc1Data2;
	uint32_t     enc2Data;
	uint32_t     enc2Data2;
	uint8_t      Direction_ENC1;
	uint8_t      Direction_ENC2;
	uint8_t      duoEnc1;
	uint8_t      duoEnc2;
} sEncoder;

typedef struct
{
	sResButtonCh          CH[NumberChannels];
	sEncoder              Encoder;
	uint16_t              ZamerCount;
    uint8_t               BlockStatus;
    uint16_t              ADC_DataCH;
    uint8_t               RearState;
} sAnalog;








#endif /* MODULE_ANALOGCONTROL_ANALOGCONTROL_H_ */
