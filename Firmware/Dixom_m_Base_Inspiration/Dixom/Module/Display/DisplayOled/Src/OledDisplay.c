#include "ssd1306.h"
#include "fonts.h"
#include <stdio.h>
#include "OledDisplay.h"
#include "TextDisplay.h"
#include "DSP_InternalCalls.h"
#include "Exchange.h"
#include "RTC.h"
#include <Structures.h>

extern sDixom Dixom;

char PointPick[1];
short OledDisplayType = 0;
short OledMenuTimer = 0;
short MenuSetChanel = 0;
uint8_t MenuSetStatus = 0;
uint8_t SetupStatus = 0 ;
uint8_t MenuDirection = 10;
short OledMenuOpenTime = 1000;

extern char TextDisplaySend[40];

char DisplaySend[100];

void LoadingOled(short staus){

	if(Dixom.Module.Display.StateOledDisplay==ON){
		SSD1306_GotoXY(25,0);
		SSD1306_Puts("LOADING", &Font_11x18, 1);
		SSD1306_DrawRectangle(0, 30, 128, 10, 1);
		SSD1306_DrawFilledRectangle(0, 30, staus, 10, 1);
		SSD1306_UpdateScreen();
		SSD1306_Fill(0);
	}
}

void OledMenuPowerDown(sDisplayMenu *DisplayMenu){

	SSD1306_GotoXY(0,20);
	SSD1306_Puts(" GOODBUY.. ", &Font_11x18, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}


void Automatic_Detection_Oled_Display_And_Initialization(void){


	if (DevState_I2C1(DEVICE_ADDR_LCD_OLED1306<<1, 1, 100) == WR_OK) {
		Dixom.Module.Display.StateOledDisplay = ON;
		SSD1306_Init();
	}else{
		Dixom.Module.Display.StateOledDisplay = OFF;
	}
}

uint8_t  ActivCallSecond = 0;
uint8_t  ActivCallMinutes = 0;
uint8_t  ActivCallHours = 0;
uint16_t ActivCallmSecond = 0;
void mSecondOledTimer(void){
	if(Dixom.Module.Bluetooth.InMode == ActiveCall){

		ActivCallmSecond++;
		if(ActivCallmSecond > 1000){
			ActivCallmSecond = 0;
			ActivCallSecond++;
			if(ActivCallSecond>59){
				ActivCallSecond=0;
				ActivCallMinutes++;
				if(ActivCallMinutes>59){
					ActivCallMinutes = 0;
					ActivCallHours++;
					if(ActivCallHours>23){
						ActivCallHours = 0;
					}
				}
			}
		}

	}else{
		ActivCallSecond  = 0;
		ActivCallMinutes = 0;
		ActivCallHours   = 0;
		ActivCallmSecond = 0;
	}

}

void OledDisplayDoneInit(void){
	if( Dixom.Module.Display.StateOledDisplay == ON ){
		OledDisplayType = GetSettings(SettingsTypeOledLCD);
	}
}

uint8_t dataVol = 0 ;

void OledTimerUpdate(tSetDisplay *SetDisplay){
	if(SetDisplay->Setup == OFF && SetDisplay->MenuBteakTimer != 0){
		SSD1306_GotoXY(90,50);
		sprintf(DisplaySend, "%hu", ((MenuBreakTime+1) - SetDisplay->MenuBteakTimer/1000)*(-1));
		SSD1306_Puts(DisplaySend, &Font_7x10, 1);
	}
}


void ClearPixel(){

	SSD1306_Fill(SSD1306_COLOR_BLACK);
}

uint8_t BluetoothTimer = 0;

void OledMenuBluetoothPhone(sDisplayMenu *DisplayMenu){

	SSD1306_Fill(0);

	dataVol = DSP_GetVolume(0);
	if(dataVol==0){
		sprintf(DisplaySend, "%s", StrMIN);
	}else if(dataVol==60){
		sprintf(DisplaySend, "%s", StrMAX);
	}else{
		sprintf(DisplaySend, "%hu", dataVol);
	}

	if(dataVol == 0){
		SSD1306_GotoXY(5,23);
		SSD1306_Puts(DisplaySend, &Font_11x18, 1);
	}else if(dataVol == 60){
		SSD1306_GotoXY(5,23);
		SSD1306_Puts(DisplaySend, &Font_11x18, 1);
	}else if(dataVol>9){
		SSD1306_GotoXY(5,21);
		SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	}else{
		SSD1306_GotoXY(15,21);
		SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	}

	SSD1306_GotoXY(0,0);
	SSD1306_Puts("  BLUETOOTH PHONE  ", &Font_7x10, 1);

	if(Dixom.Module.Bluetooth.InMode == IncomingCall){
		SSD1306_GotoXY(49,23);
		SSD1306_Puts("Incoming", &Font_7x10, 1);
		SSD1306_GotoXY(49,35);
		SSD1306_Puts("Call", &Font_7x10, 1);
	}else if(Dixom.Module.Bluetooth.InMode == OutgoingCall){
		SSD1306_GotoXY(49,23);
		SSD1306_Puts("Outgoing", &Font_7x10, 1);
		SSD1306_GotoXY(49,35);
		SSD1306_Puts("Call", &Font_7x10, 1);
	}else{
		SSD1306_GotoXY(49,23);
		SSD1306_Puts("Active Call", &Font_7x10, 1);
		SSD1306_GotoXY(49,35);
		sprintf(DisplaySend, "%.2d:%.2d:%.2d", ActivCallHours,ActivCallMinutes,ActivCallSecond);
		SSD1306_Puts(DisplaySend, &Font_7x10, 1);
	}


	SSD1306_GotoXY(0,53);
	sprintf(DisplaySend, "%s", Dixom.Module.Bluetooth.ConnDevInfo.LastPhoneNumber);
	SSD1306_Puts(DisplaySend, &Font_7x10, 1);

	SSD1306_UpdateScreen();

}

void OledMenuMain(sDisplayMenu *DisplayMenu){
	char Timeb[8];

	if(GetSettings(SettingsTypeOledLCD) == Oled_Menu_Type1){  //��� ���� ������������ �� LCD

		//Dixom.Module.Display.LcdUpdate = 0;
		SSD1306_GotoXY(0,0);

		dataVol = DSP_GetVolume(0);
		if(dataVol==0){
			sprintf(DisplaySend, "%s", StrMIN);
		}else if(dataVol==60){
			sprintf(DisplaySend, "%s", StrMAX);
		}else{
			sprintf(DisplaySend, "%hu", dataVol);
		}




		//Font_7x10
		//Font_16x26

		switch (Dixom.Module.DSP.Settings[SoundSource].Data){

		case SoursUsb        :   SSD1306_Puts("USB", &Font_11x18, 1);

		if(Dixom.Exchange.State.USB.UsbConnection == Connected){
			SSD1306_DrawRectangle(53, 4, 8, 3, 1);
			SSD1306_DrawFilledRectangle(55, 1, 4, 3, 1);
			SSD1306_DrawFilledRectangle(55, 11, 4, 2, 1);
			SSD1306_DrawRectangle(53, 8, 8, 3, 1);

		}else{
			SSD1306_DrawRectangle(53, 1, 8, 3, 1);
			SSD1306_DrawFilledRectangle(54, 7, 2, 3, 1);
			SSD1306_DrawFilledRectangle(58, 7, 2, 3, 1);
			SSD1306_DrawRectangle(53, 10, 8, 3, 1);

		}


/*
		if(Dixom.ExchangeState.UsbConnection == 0){
			SSD1306_DrawRectangle(37, 1, 8, 3, 1);
			SSD1306_DrawFilledRectangle(38, 7, 2, 3, 1);
			SSD1306_DrawFilledRectangle(42, 7, 2, 3, 1);
			SSD1306_DrawRectangle(37, 10, 8, 3, 1);
		}else{
			SSD1306_DrawRectangle(37, 4, 8, 3, 1);
			SSD1306_DrawFilledRectangle(39, 1, 4, 3, 1);
			SSD1306_DrawFilledRectangle(39, 11, 4, 2, 1);
			SSD1306_DrawRectangle(37, 8, 8, 3, 1);
		}


		if(UsbAudioConState() == 0){
			SSD1306_DrawRectangle(55, 1, 8, 3, 1);
			SSD1306_DrawFilledRectangle(56, 7, 2, 3, 1);
			SSD1306_DrawFilledRectangle(60, 7, 2, 3, 1);
			SSD1306_DrawRectangle(55, 10, 8, 3, 1);
		}else{
			SSD1306_DrawRectangle(55, 4, 8, 3, 1);
			SSD1306_DrawFilledRectangle(57, 1, 4, 3, 1);
			SSD1306_DrawFilledRectangle(57, 11, 4, 2, 1);
			SSD1306_DrawRectangle(55, 8, 8, 3, 1);
		}*/

		if(dataVol == 0){
			SSD1306_GotoXY(5,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol == 60){
			SSD1306_GotoXY(5,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol>9){
			SSD1306_GotoXY(5,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}else{
			SSD1306_GotoXY(15,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}









/*
		SSD1306_GotoXY(53,23);
		if(UsbAudioBitRate() == 1){
			SSD1306_Puts("16-BIT", &Font_7x10, 1);
		}else if(UsbAudioBitRate() == 2){
			SSD1306_Puts("24-BIT", &Font_7x10, 1);
		}else if(UsbAudioBitRate() == 3){
			SSD1306_Puts("32-BIT", &Font_7x10, 1);
		}else{
			SSD1306_Puts("------", &Font_7x10, 1);
		}

		SSD1306_GotoXY(53,35);
		if(UsbAudioSamplingFreq() == 1){
			SSD1306_Puts("44100-Hz", &Font_7x10, 1);
		}else if(UsbAudioSamplingFreq() == 2){
			SSD1306_Puts("48000-Hz", &Font_7x10, 1);
		}else if(UsbAudioSamplingFreq() == 3){
			SSD1306_Puts("88200-Hz", &Font_7x10, 1);
		}else if(UsbAudioSamplingFreq() == 4){
			SSD1306_Puts("96000-Hz", &Font_7x10, 1);
		}else{
			SSD1306_Puts("------", &Font_7x10, 1);
		}
*/
		//SSD1306_GotoXY(53,35);
		//SSD1306_Puts("96000-Hz", &Font_7x10, 1);

		//SSD1306_GotoXY(0,53);
		//SSD1306_Puts("DIXOM HI RES AUDIO", &Font_7x10, 1);
		//SSD1306_Puts("HI RES AUDIO", &Font_7x10, 1);



		SSD1306_DrawRectangle(53, 25, 64, 16, 1);
		SSD1306_DrawFilledRectangle(55, 27, Dixom.Module.DSP.Volume[0].DataVol, 12, 1);

		SSD1306_DrawRectangle(53, 53, 64, 10, 1);
		uint8_t lDetect = Dixom.Module.DSP.LvlDetec[0].data/4.4;
		SSD1306_DrawFilledRectangle(55, 55, lDetect, 6, 1);

		SSD1306_GotoXY(4,53);
		sprintf(DisplaySend, "%.1fV", (Dixom.Service.ACC.Voltage*0.1));

		if(Dixom.Service.PeripheryState[Remout] == ON){
			SSD1306_Puts(DisplaySend, &Font_7x10, 1);
		}else{
			SSD1306_Puts(DisplaySend, &Font_7x10, 0);
		}



		break;
		case SoursBluetoooth :   SSD1306_Puts("BT", &Font_11x18, 1);


		if(dataVol == 0){
			SSD1306_GotoXY(5,23);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol == 60){
			SSD1306_GotoXY(5,23);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol>9){
			SSD1306_GotoXY(5,21);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}		else{
			SSD1306_GotoXY(15,21);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}

		//SSD1306_DrawRectangle(48, 20, 79, 22, 1);
		if(Dixom.Module.Bluetooth.InMode == Connected){
			SSD1306_DrawRectangle(53, 4, 8, 3, 1);
			SSD1306_DrawFilledRectangle(55, 1, 4, 3, 1);
			SSD1306_DrawFilledRectangle(55, 11, 4, 2, 1);
			SSD1306_DrawRectangle(53, 8, 8, 3, 1);

		}else{
			SSD1306_DrawRectangle(53, 1, 8, 3, 1);
			SSD1306_DrawFilledRectangle(54, 7, 2, 3, 1);
			SSD1306_DrawFilledRectangle(58, 7, 2, 3, 1);
			SSD1306_DrawRectangle(53, 10, 8, 3, 1);

		}

		SSD1306_GotoXY(50,19);

		if(BluetoothTimer < 50){
			BluetoothTimer++;
			sprintf(DisplaySend, "%s", Dixom.Module.Bluetooth.ConnDevInfo.Name);

			if(Dixom.Module.Bluetooth.InMode == Connected){
				SSD1306_Puts(DisplaySend, &Font_11x18, 1);
			}else{
				SSD1306_Puts("-------", &Font_11x18, 1);
			}
		}else if(BluetoothTimer <70){
			BluetoothTimer++;
			if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_NO){
				SSD1306_Puts("-------", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_CBC){
				SSD1306_Puts("  CBS  ", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_MP3){
				SSD1306_Puts("  MP3  ", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_AAC){
				SSD1306_Puts("  AAC  ", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_Fastream){
				SSD1306_Puts("Fstream", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_AptX){
				SSD1306_Puts(" AptX  ", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_AptX_St){
				SSD1306_Puts("AptX-St", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_AptX_HD){
				SSD1306_Puts("AptX-HD", &Font_11x18, 1);
			}else if(Dixom.Module.Bluetooth.ConnDevInfo.A2dpDecoder == BTDecoder_AptX_LL){
				SSD1306_Puts("AptX-LL", &Font_11x18, 1);
			}
		}else{
			BluetoothTimer = 0;
		}


		SSD1306_GotoXY(92,36);
		sprintf(DisplaySend, "%.1fV", (Dixom.Service.ACC.Voltage*0.1));
		//SSD1306_Puts(DisplaySend, &Font_7x10, 1);

		if(Dixom.Service.PeripheryState[Remout] == ON){
			SSD1306_Puts(DisplaySend, &Font_7x10, 1);
		}else{
			SSD1306_Puts(DisplaySend, &Font_7x10, 0);
		}


		SSD1306_DrawRectangle(26, 11, 3, 3, 1);
		SSD1306_DrawRectangle(29, 9, 3,  5, 1);
		SSD1306_DrawRectangle(32, 6, 3,  8, 1);
		SSD1306_DrawRectangle(35, 3, 3,  11, 1);
		SSD1306_DrawRectangle(38, 1, 3,  13, 1);

		//uint8_t signal =  GetNetvorkSignal();

		if( Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl == '1'){
			SSD1306_DrawFilledRectangle(26, 11, 3, 3, 1);
		}else if(Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl =='2'){
			SSD1306_DrawFilledRectangle(26, 11, 3, 3, 1);
			SSD1306_DrawFilledRectangle(29, 9, 3, 5, 1);
		}else if(Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl =='3'){
			SSD1306_DrawFilledRectangle(26, 11, 3, 3, 1);
			SSD1306_DrawFilledRectangle(29, 9, 3, 5, 1);
			SSD1306_DrawFilledRectangle(32, 6, 3, 8, 1);
		}else if(Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl =='4'){
			SSD1306_DrawFilledRectangle(26, 11, 3, 3, 1);
			SSD1306_DrawFilledRectangle(29, 9, 3, 5, 1);
			SSD1306_DrawFilledRectangle(32, 6, 3, 8, 1);
			SSD1306_DrawFilledRectangle(35, 3, 3, 11, 1);
		}else if(Dixom.Module.Bluetooth.ConnDevInfo.NetworkLevl =='5'){
			SSD1306_DrawFilledRectangle(26, 11, 3, 3, 1);
			SSD1306_DrawFilledRectangle(29, 9, 3, 5, 1);
			SSD1306_DrawFilledRectangle(32, 6, 3, 8, 1);
			SSD1306_DrawFilledRectangle(35, 3, 3, 11, 1);
			SSD1306_DrawFilledRectangle(38, 1, 3, 13, 1);
		}

		SSD1306_DrawRectangle(0, 57, 128, 7, 1);




		int progress = Dixom.Module.Bluetooth.ConnDevInfo.TimeProgress;
		int total = Dixom.Module.Bluetooth.ConnDevInfo.TimeTotal;

		int res_hour = 0;
		int res_min = 0;
		int res_sec = 0;
		if (progress != -1)
		{
			res_min = progress / 60000;
			res_sec = ((progress / 600) - (res_min * 100)) * 60 / 100;
			res_hour = res_min / 60;
			res_min = res_hour > 0 ? (res_min - (res_hour * 60)) : res_min;
		}

		int res_hour_total = 0;
		int res_min_total = 0;
		int res_sec_total = 0;
		if (total != -1)
		{
			res_min_total = total / 60000;
			res_sec_total = ((total / 600) - (res_min_total * 100)) * 60 / 100;
			res_hour_total = res_min_total / 60;
			res_min_total = res_hour_total > 0 ? (res_min_total - (res_hour_total * 60)) : res_min_total;
		}

		sprintf(DisplaySend, "%.2d:%.2d:%.2d", res_hour,res_min,res_sec);
		SSD1306_GotoXY(0,47);
		SSD1306_Puts(DisplaySend, &Font_7x10, 1);
		sprintf(DisplaySend, "%.2d:%.2d:%.2d", res_hour_total,res_min_total,res_sec_total);
		SSD1306_GotoXY(71,47);
		SSD1306_Puts(DisplaySend, &Font_7x10, 1);
		if(total == 0){
			total = 1;
		}
		SSD1306_DrawFilledRectangle(0, 57, progress*127/total, 7, 1);



		break;
		case SoursAux        :   SSD1306_Puts("AUX", &Font_11x18, 1);

		if(dataVol == 0){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol == 60){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol>9){
			SSD1306_GotoXY(40,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}else{
			SSD1306_GotoXY(55,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}

		SSD1306_GotoXY(0,53);
		SSD1306_Puts("LINEAR AUDIO INPUT", &Font_7x10, 1);

		break;
		case SoursRadio      :   SSD1306_Puts("RADIO", &Font_11x18, 1);
		if(dataVol == 0){
				SSD1306_GotoXY(5,25);
				SSD1306_Puts(DisplaySend, &Font_11x18, 1);
			}else if(dataVol == 60){
				SSD1306_GotoXY(5,25);
				SSD1306_Puts(DisplaySend, &Font_11x18, 1);
			}else if(dataVol>9){
				SSD1306_GotoXY(5,23);
				SSD1306_Puts(DisplaySend, &Font_16x26, 1);
			}else{
				SSD1306_GotoXY(15,23);
				SSD1306_Puts(DisplaySend, &Font_16x26, 1);
			}

			SSD1306_GotoXY(49,25);

			// Dixom.Module.FmRadio.Freq = freq;%.2f


			sprintf(Timeb, "%.1fFM", Dixom.Module.FmRadio.FmPreset[0].Freq*FConv) ;
			SSD1306_Puts(Timeb, &Font_11x18, 1);

			SSD1306_GotoXY(0,53);
			SSD1306_Puts("No RDS data........", &Font_7x10, 1);

		break;
		case SoursSpdif      :   SSD1306_Puts("SPDIF", &Font_11x18, 1);

		if(dataVol == 0){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol == 60){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol>9){
			SSD1306_GotoXY(40,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}else{
			SSD1306_GotoXY(55,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}

		SSD1306_GotoXY(0,53);
		SSD1306_Puts("TOSLINK S/PDIF IN", &Font_7x10, 1);
		break;
		case SoursMic        :   SSD1306_Puts("MIC", &Font_11x18, 1);
		if(dataVol == 0){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol == 60){
			SSD1306_GotoXY(40,25);
			SSD1306_Puts(DisplaySend, &Font_11x18, 1);
		}else if(dataVol>9){
			SSD1306_GotoXY(40,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}else{
			SSD1306_GotoXY(55,23);
			SSD1306_Puts(DisplaySend, &Font_16x26, 1);
		}

		SSD1306_GotoXY(0,53);
		SSD1306_Puts("DIGITAL MICROPHONE", &Font_7x10, 1);
		break;
		}

/*
		if( Dixom.Module.DSP.Settings[SoundSource].Data == SoursUsb){
			sprintf(Timeb, "%02hhu:%02hhu:%02hhu", Dixom.DevInfo.DateTime.Hours,Dixom.DevInfo.DateTime.Minutes,Dixom.DevInfo.DateTime.Seconds) ;
			SSD1306_GotoXY(38,0);
		}else{
			sprintf(Timeb, "%02hhu%s%02hhu", Dixom.DevInfo.DateTime.Hours,PointPick,Dixom.DevInfo.DateTime.Minutes) ;
			SSD1306_GotoXY(71,0);
		}*/

		sprintf(Timeb, "%02hhu%s%02hhu", Dixom.DevInfo.DateTime.Hours,PointPick,Dixom.DevInfo.DateTime.Minutes) ;
		SSD1306_GotoXY(71,0);




		SSD1306_Puts(Timeb, &Font_11x18, 1);

		SSD1306_UpdateScreen();
		SSD1306_Fill(0);
		SetupStatus = 0;
	}
}



void OledDisplayEvent(sDisplayMenu *DisplayMenu) {

	SSD1306_GotoXY(0,0);

	if (DisplayMenu->Event.type == EventBlink) {

		SSD1306_Puts("  STATIK", &Font_11x18, 1);

	} else if (DisplayMenu->Event.type == EventStatik) {

		if (DisplayMenu->Event.whence == ChangeSource) {

			SSD1306_Puts("   SOURCE   ", &Font_11x18, 1);
			SSD1306_GotoXY(0,30);
			switch (Dixom.Module.DSP.Settings[SoundSource].Data) {

			case SoursUsb:
				SSD1306_GotoXY(5,30);
				SSD1306_Puts(" USB AUDIO ", &Font_11x18, 1);
				break;
			case SoursBluetoooth:
				SSD1306_GotoXY(5,30);
				SSD1306_Puts(" BLUETOOTH ", &Font_11x18, 1);
				break;
			case SoursAux:
				SSD1306_GotoXY(5,30);
				SSD1306_Puts(" AUX INPUT ", &Font_11x18, 1);
				break;
			case SoursRadio:
				SSD1306_GotoXY(5,30);
				SSD1306_Puts("   RADIO   ", &Font_11x18, 1);
				break;
			case SoursSpdif:
				SSD1306_GotoXY(5,30);
				SSD1306_Puts("SPDIF INPUT", &Font_11x18, 1);
				break;
			case SoursMic:
				SSD1306_GotoXY(0,30);

				SSD1306_Puts(" MICROPHONE ", &Font_11x18, 1);
				break;
			}
		}
		if (DisplayMenu->Event.whence == ChangeVolume) {

			if (DisplayMenu->Param.Num[0] == VolGroup1) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(0));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup2) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(1));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup3) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(2));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup4) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(3));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup5) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(4));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup6) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(5));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolGroup7) {
				sprintf(TextDisplaySend, "%s               ", DSP_GetGroupName(6));
				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursUsb) {
				SSD1306_Puts("VOLUM USB", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursBluetoooth) {
				SSD1306_Puts("VOLUM BT", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursAux) {
				SSD1306_Puts("VOLUM AUX", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursRadio) {
				SSD1306_Puts("VOLUM FM", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursSpdif) {
				SSD1306_Puts("VOLUM SPDIF", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == VolSoursMic) {
				SSD1306_Puts("VOLUM MIC", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolUsb) {
				SSD1306_Puts("MinVOL USB", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolBluetoooth) {
				SSD1306_Puts("MinVOL BT", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolAux) {
				SSD1306_Puts("MinVOL AUX", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolsRadio) {
				SSD1306_Puts("MinVOL FM", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolSpdif) {
				SSD1306_Puts("MinVOL SPDIF", &Font_11x18, 1);
			} else if (DisplayMenu->Param.Num[0] == MinVolMic) {
				SSD1306_Puts("MinVOL MIC", &Font_11x18, 1);
			} else {
				sprintf(TextDisplaySend, "VOLUM %d",
						DisplayMenu->Param.Num[0]);

				SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
			}

			SSD1306_GotoXY(0,30);

			sprintf(TextDisplaySend, "Volume %hu      ",
					DisplayMenu->Param.Num[1]);

			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);

		}

	} else {

	}

	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}


void OledMenuLevel2_1(sDisplayMenu *DisplayMenu){
	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Fast ", &Font_11x18, 1);

	switch (DisplayMenu->MenuLevel_2){
	case 1 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Back", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Source", &Font_11x18, 1);
		break;
	case 2 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Source", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Group vol", &Font_11x18, 1);
		break;
	case 3 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Group vol", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Brightness", &Font_11x18, 1);
		break;
	case 4 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Brightness", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Back", &Font_11x18, 1);
		break;
	}

	SSD1306_UpdateScreen();

}


void OledGroupVol(sDisplayMenu *DisplayMenu) {
	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Group vol ", &Font_11x18, 1);
	SSD1306_GotoXY(0,20);

	switch (DisplayMenu->MenuLevel_3) {
	case 1:
		SSD1306_Puts(">Back", &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		sprintf(TextDisplaySend, " %s", DSP_GetGroupName(0));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		break;
	case 2:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(0));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);

		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(29));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(1));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 3:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(1));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(32));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(2));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 4:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(2));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(35));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(3));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 5:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(3));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(38));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(4));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 6:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(4));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(41));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(5));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 7:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(5));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(44));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			sprintf(TextDisplaySend, " %s", DSP_GetGroupName(6));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}
		break;
	case 8:
		sprintf(TextDisplaySend, ">%s", DSP_GetGroupName(6));
		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		SSD1306_GotoXY(0,40);
		if(DisplayMenu->MenuStatus == Status_Setup){
			sprintf(TextDisplaySend, "      %hhu ", DSP_GetVolume(47));
			SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
		}else{
			SSD1306_GotoXY(0,40);
			SSD1306_Puts(" Back", &Font_11x18, 1);
		}
		break;
	}
	SSD1306_UpdateScreen();

}


void OledBrightness(sDisplayMenu *DisplayMenu){
	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Fast ", &Font_11x18, 1);

	if (DisplayMenu->MenuStatus != Status_Setup) {
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Brightness", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Back", &Font_11x18, 1);
	} else {

		SSD1306_GotoXY(0,0); SSD1306_Puts(">Brightness", &Font_11x18, 1);

		if(Dixom.Module.Display.Brightness==0){
			sprintf(TextDisplaySend, "  %s    ", StrMIN);
		}else if(Dixom.Module.Display.Brightness==16){
			sprintf(TextDisplaySend, "  %s    ", StrMAX);
		}else{
			sprintf(TextDisplaySend, "  %d    ", Dixom.Module.Display.Brightness);
		}
		SSD1306_GotoXY(0,25);

		SSD1306_Puts(TextDisplaySend, &Font_11x18, 1);
	}
	SSD1306_UpdateScreen();

}

void OledSourceSetup(sDisplayMenu *DisplayMenu){

	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	if (DisplayMenu->MenuStatus != Status_Setup) {
		SSD1306_Puts("Fast", &Font_11x18, 1);
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Source", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Group vol", &Font_11x18, 1);
	} else {

		SSD1306_Puts(">Source Set", &Font_11x18, 1);
		SSD1306_GotoXY(0,30);
		switch (Dixom.Module.DSP.Settings[SoundSource].Data) {

		case SoursUsb:
			SSD1306_Puts(" USB AUDIO ", &Font_11x18, 1);
			break;
		case SoursBluetoooth:
			SSD1306_Puts(" BLUETOOTH ", &Font_11x18, 1);
			break;
		case SoursAux:
			SSD1306_Puts(" AUX INPUT ", &Font_11x18, 1);
			break;
		case SoursRadio:
			SSD1306_Puts("   RADIO   ", &Font_11x18, 1);
			break;
		case SoursSpdif:
			SSD1306_Puts("SPDIF INPUT", &Font_11x18, 1);
			break;
		case SoursMic:
			SSD1306_Puts("MICROPHONE ", &Font_11x18, 1);
			break;
		}
	}

	SSD1306_UpdateScreen();

}


void OledMenuLevel1(sDisplayMenu *DisplayMenu){


	SSD1306_Fill(0);
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("Dixom menu", &Font_11x18, 1);


	switch (DisplayMenu->MenuLevel_1){
	case 1 :                             //   Dixom menu
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Back", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Fast", &Font_11x18, 1);
		break;
	case 2 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Fast", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Sound", &Font_11x18, 1);
		break;
	case 3 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Sound", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Platform", &Font_11x18, 1);
	break;
	case 4 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Platform", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Reinit", &Font_11x18, 1);
		break;
	case 5 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Reinit", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Error View", &Font_11x18, 1);
		break;
	case 6 :
		SSD1306_GotoXY(0,20); SSD1306_Puts(">Error View", &Font_11x18, 1);
		SSD1306_GotoXY(0,40); SSD1306_Puts(" Back", &Font_11x18, 1);
		break;
	}

	SSD1306_UpdateScreen();




}

/*

void OledMenuSource(tSetDisplay *SetDisplay){

	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}
	uint8_t source = GetSettings(SettingsSoursSelect);
	SSD1306_GotoXY(30,0);
	SSD1306_Puts("SOURCE", &Font_11x18, 1);
	SSD1306_GotoXY(5,21);

	switch (source){

	case SoursUsb        : SSD1306_GotoXY(45,30);    SSD1306_Puts("USB", &Font_11x18, 1);             break;
	case SoursBluetoooth : SSD1306_GotoXY(15,30);    SSD1306_Puts("BLUETOOTH", &Font_11x18, 1);       break;
	case SoursAux        : SSD1306_GotoXY(45,30);    SSD1306_Puts("AUX", &Font_11x18, 1);             break;
	case SoursRadio      : SSD1306_GotoXY(35,30);    SSD1306_Puts("RADIO", &Font_11x18, 1);           break;
	case SoursSpdif      : SSD1306_GotoXY(35,30);    SSD1306_Puts("SPDIF", &Font_11x18, 1);           break;
	case SoursMic        : SSD1306_GotoXY(35,30);    SSD1306_Puts("MIC", &Font_11x18, 1);             break;


	}

	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}

void OledEventVolume(tSetDisplay *SetDisplay) {

	SSD1306_GotoXY(0, 0);
	if (SetDisplay->Param1 == VolGroup1) {
		sprintf(DisplaySend, "GROUP VOL 1");
	} else if (SetDisplay->Param1 == VolGroup2) {
		sprintf(DisplaySend, "GROUP VOL 2");
	} else if (SetDisplay->Param1 == VolGroup3) {
		sprintf(DisplaySend, "GROUP VOL 3");
	} else if (SetDisplay->Param1 == VolGroup4) {
		sprintf(DisplaySend, "GROUP VOL 4");
	} else if (SetDisplay->Param1 == VolGroup5) {
		sprintf(DisplaySend, "GROUP VOL 5");
	} else if (SetDisplay->Param1 == VolGroup6) {
		sprintf(DisplaySend, "GROUP VOL 6");
	} else if (SetDisplay->Param1 == VolGroup7) {
		sprintf(DisplaySend, "VOL SPDIF");
	} else if (SetDisplay->Param1 == VolSoursUsb) {
		sprintf(DisplaySend, "VOLUME USB");
	} else if (SetDisplay->Param1 == VolSoursBluetoooth) {
		sprintf(DisplaySend, "VOLUME BT");
	} else if (SetDisplay->Param1 == VolSoursAux) {
		sprintf(DisplaySend, "VOLUME AUX");
	} else if (SetDisplay->Param1 == VolSoursRadio) {
		sprintf(DisplaySend, "VOLUME RADIO");
	} else if (SetDisplay->Param1 == VolSoursSpdif) {
		sprintf(DisplaySend, "VOLUME SPDIF");
	} else if (SetDisplay->Param1 == VolSoursMic) {
		sprintf(DisplaySend, "VOLUME MIC");
	} else if (SetDisplay->Param1 == MinVolUsb) {
		sprintf(DisplaySend, "MIN VOL USB");
	} else if (SetDisplay->Param1 == MinVolBluetoooth) {
		sprintf(DisplaySend, "MIN VOL BT");
	} else if (SetDisplay->Param1 == MinVolAux) {
		sprintf(DisplaySend, "MIN VOL AUX");
	} else if (SetDisplay->Param1 == MinVolsRadio) {
		sprintf(DisplaySend, "MIN VOL FM");
	} else if (SetDisplay->Param1 == MinVolSpdif) {
		sprintf(DisplaySend, "MIN VOL SPDIF");
	} else if (SetDisplay->Param1 == MinVolMic) {
		sprintf(DisplaySend, "MIN VOL MIC");
	} else {
		sprintf(DisplaySend, "VOLUME %d", SetDisplay->Param1);

	}

	SSD1306_Puts(DisplaySend, &Font_11x18, 1);

	sprintf(DisplaySend, "%d", SetDisplay->Param2);
	SSD1306_GotoXY(45, 26);
	SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);

}


void OledMenuBrightness(tSetDisplay *SetDisplay){

	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}

	SSD1306_GotoXY(0,0);
	SSD1306_Puts("BRIGHTNESS", &Font_11x18, 1);

	if(GetSettings(SettingsDisplayBrightness)==0){
		sprintf(DisplaySend, "%s", StrMIN);
	}else if(GetSettings(SettingsDisplayBrightness)==16){
		sprintf(DisplaySend, "%s", StrMAX);
	}else{
		sprintf(DisplaySend, "%d", GetSettings(SettingsDisplayBrightness));
	}

	SSD1306_GotoXY(45,26);
	SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}

void OledMenuLoudness(tSetDisplay *SetDisplay){

}

void OledMenuTimeset(tSetDisplay *SetDisplay){

}

void OledMenuVolGroup(tSetDisplay *SetDisplay, short VolNum){

	int8_t data;

	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}

	SSD1306_GotoXY(0,0);

	switch (VolNum){
	case 29      :	data = DSP_GetVolume(29);	SSD1306_Puts("GROUP 1", &Font_11x18, 1);		break;
	case 32      :	data = DSP_GetVolume(32);	SSD1306_Puts("GROUP 2", &Font_11x18, 1);		break;
	case 35      :	data = DSP_GetVolume(35);	SSD1306_Puts("GROUP 3", &Font_11x18, 1);		break;
	case 38      :	data = DSP_GetVolume(38);	SSD1306_Puts("GROUP 4", &Font_11x18, 1);		break;
	case 41      :	data = DSP_GetVolume(41);	SSD1306_Puts("GROUP 5", &Font_11x18, 1);		break;
	case 44      :  data = DSP_GetVolume(44);	SSD1306_Puts("GROUP 6", &Font_11x18, 1);		break;
	case 47      :	data = DSP_GetVolume(47);	SSD1306_Puts("SPDIF", &Font_11x18, 1);		    break;
	}

	data = DSP_GetVolume(VolNum);


	if(data==0){
		sprintf(DisplaySend, "%s", StrMIN);
	}else if(data==60){
		sprintf(DisplaySend, "%s", StrMAX);
	}else{
		sprintf(DisplaySend, "%hu", data);
	}


	SSD1306_GotoXY(45,26);
	SSD1306_Puts(DisplaySend, &Font_16x26, 1);
	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}

void OledMenuPresetEq(tSetDisplay *SetDisplay){

	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}

	SSD1306_GotoXY(0,0);
	SSD1306_Puts("EQ PRESSET", &Font_11x18, 1);
	SSD1306_GotoXY(0,30);
	switch (GetSettings(SettingsEQPreset)){
	case 0       :  SSD1306_Puts("PRESSET OFF", &Font_11x18, 1);    break;
	case 1       :  SSD1306_Puts("USER 1", &Font_11x18, 1);         break;
	case 2       :  SSD1306_Puts("USER 2", &Font_11x18, 1);         break;
	case 3       :  SSD1306_Puts("USER 3", &Font_11x18, 1);         break;
	case 4       :  SSD1306_Puts("USER 4", &Font_11x18, 1);         break;
	case 5       :  SSD1306_Puts("USER 5", &Font_11x18, 1);         break;
	case 6       :  SSD1306_Puts("FLAT", &Font_11x18, 1);           break;
	case 7       :  SSD1306_Puts("BASS", &Font_11x18, 1);           break;
	case 8       :  SSD1306_Puts("BASS EXTREM", &Font_11x18, 1);    break;
	case 9       :  SSD1306_Puts("BASS AND HIGH", &Font_11x18, 1);  break;
	case 10      :  SSD1306_Puts("HIGH", &Font_11x18, 1);           break;
	case 11      :  SSD1306_Puts("CLASSIC", &Font_11x18, 1);        break;
	case 12      :  SSD1306_Puts("DANCING", &Font_11x18, 1);        break;
	case 13      :  SSD1306_Puts("ROCK", &Font_11x18, 1);           break;
	case 14      :  SSD1306_Puts("TEHNO", &Font_11x18, 1);          break;
	case 15      :  SSD1306_Puts("SPEAKER", &Font_11x18, 1);        break;
	case 16      :  SSD1306_Puts("LIVE", &Font_11x18, 1);           break;
	case 17      :  SSD1306_Puts("MEDIUM", &Font_11x18, 1);         break;
	case 18      :  SSD1306_Puts("SOFT", &Font_11x18, 1);           break;
	case 19      :  SSD1306_Puts("SOFT BASS", &Font_11x18, 1);      break;
	case 20      :  SSD1306_Puts("SOFT HIGH", &Font_11x18, 1);      break;
	}

	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}
void OledMenuPresetCross(tSetDisplay *SetDisplay){

	if(SetDisplay->Setup == ON){
		SSD1306_GotoXY(90,50);
		SSD1306_Puts("SETUP", &Font_7x10, 1);
	}
	SSD1306_GotoXY(0,0);
	SSD1306_Puts("CROSS PRESSET", &Font_11x18, 1);
	SSD1306_GotoXY(0,30);
	switch (GetSettings(SettingsCROSSPreset)){
	case 0      :  SSD1306_Puts("PRESSET OFF", &Font_11x18, 1);    break;
	case 1      :  SSD1306_Puts("USER 1", &Font_11x18, 1);         break;
	case 2      :  SSD1306_Puts("USER 2", &Font_11x18, 1);         break;
	case 3      :  SSD1306_Puts("USER 3", &Font_11x18, 1);         break;
	case 4      :  SSD1306_Puts("USER 4", &Font_11x18, 1);         break;
	case 5      :  SSD1306_Puts("USER 5", &Font_11x18, 1);         break;
	}

	SSD1306_UpdateScreen();
	SSD1306_Fill(0);
}
*/

