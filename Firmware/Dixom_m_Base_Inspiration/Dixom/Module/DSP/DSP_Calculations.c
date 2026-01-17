#include <Math.h>
#include <Structures.h>
#include "DSP_Calculations.h"
#include "DSP_Exchange.h"

extern sDixom Dixom;


#define dDSP Dixom.Module.DSP

union  {
    int val;
    uint8_t  bytes[4];
} UdspData;

union  {
    int val;
    uint8_t  bytes[1];
} UdacData;


void DSP_setFloatPointInDataArray( float val, uint8_t* result ){
	UdspData.val = val * pow(2,24);

	for ( short i = 3; i>=0; i--){
	   result[3-i]= UdspData.bytes[i];
	}
}

void DSP_set5PointInDataArray( float val1, float val2, float val3, float val4, float val5, uint8_t* result){
  DSP_setFloatPointInDataArray(val1, result   );
  DSP_setFloatPointInDataArray(val2, result+4 );
  DSP_setFloatPointInDataArray(val3, result+8 );
  DSP_setFloatPointInDataArray(val4, result+12);
  DSP_setFloatPointInDataArray(val5, result+16);
}

void DSP_CalculateFilter_POINT ( sFiltrParam point, uint8_t* result ){

	double omega = 0;
	double A = 0;
	double S = 1;
	double G19 =0;
	double gainLinear = 0;
	double gain = 0;
	double alpha=0;

	double Q = point.Q * CONV_FLOAT ;
	double Boost = point.Boost * CONV_FLOAT;



	if(point.OnOff==FILTER_OFF){
		point.Type=FILTER_OFF;
	}

	if (point.Type!=FILTER_OFF){
	   if( Q == 0){ Q = 1e-9; }
	   omega = 2 * M_PI * point.Freq / dDSP.SAMPLE_RATE ;

	   gainLinear = pow( 10, GAIN_CONST / 20 );
	}

	double B2 = 0;
	double B1 = 0;
	double B0 = 1;
	double A2 = 0;
	double A1 = 0;
	double A0 = 1;

	double CosOmega = cos(omega);
	double SinOmega = sin(omega);

	switch (point.Type) {
      case FILTER_OFF:
    	  break;
      case PEAK:
    	  if(Boost!=0){
    	  A = pow(10, Boost / 40);
    	  alpha = SinOmega / (2 * Q);
          A0 =   1 + alpha / A;
	      A1 =  -2 * CosOmega/A0*(-1);
	      A2 =  (1 - alpha / A)/A0*(-1);
	      B0 =  (1 + alpha * A)*gainLinear/A0;
	      B1 =  -2 * CosOmega*gainLinear/A0;
	      B2 =  (1 - alpha * A)*gainLinear/A0;
    	  }
	      break;
      case LOW_SHELF:

    	  A          = pow( 10, Boost / 40 );
    	  alpha      = SinOmega/2 * sqrt((A + 1/A)*(1/S - 1) + 2);
    	  G19        = SinOmega * sqrt((A*A+1)*(1/S-1)+2*A );
    	  A0         = (A+1) + (A-1) * CosOmega + G19 ;
    	  gainLinear = pow( 10, gain / 20 )/A0;

          A1 =  2*((A-1) + (A+1)*CosOmega)/A0;
          A2 =  ((A+1+(A-1)*CosOmega-G19)/A0)*(-1);
          B0 =  (A*((A+1)-(A-1)*CosOmega+G19))*gainLinear;
          B1 =  2*A*((A-1)-(A+1)*CosOmega)*gainLinear;
          B2 =  A*((A+1) - (A-1)*CosOmega-G19)*gainLinear;

          break;
      case HIGH_SHELF:
    	  A          = pow( 10, Boost / 40 );
    	  alpha      = SinOmega/2 * sqrt((A + 1/A)*(1/S - 1) + 2);
    	  G19        = SinOmega * sqrt((A*A+1)*(1/S-1)+2*A );
    	  A0         = (A+1) + (1-A) * CosOmega + G19 ;
    	  gainLinear = pow( 10, gain / 20 )/A0;


          A1 =  2*((A-1) - (A+1)*CosOmega)/A0*(-1);
          A2 =  ((A+1+(1-A)*CosOmega-G19)/A0)*(-1);
          B0 =  (A*((A+1)+(A-1)*CosOmega+G19))*gainLinear;
          B1 =  -2*A*((A-1)+(A+1)*CosOmega)*gainLinear;
          B2 =  A*((A+1) + (A-1)*CosOmega-G19)*gainLinear;
          break;
	     /*
      case LOW_SHELF:
    	  A = sqrt(pow(10, point.Boost / 20));
    	  S = sqrt(A + A);
    	  alpha = SinOmega/2 * sqrt((A + 1/A)*(1/S - 1) + 2);

          A0 =       (A+1) + (A-1)*CosOmega + 2*sqrt(A)*alpha;
          A1 =   -2*((A-1) + (A+1)*CosOmega);
          A2 =       (A+1) + (A-1)*CosOmega - 2*sqrt(A)*alpha;
          B0 =    A*((A+1) - (A-1)*CosOmega + 2*sqrt(A)*alpha ) * gainLinear;
          B1 =  2*A*((A-1) - (A+1)*CosOmega)* gainLinear;
          B2 =    A*((A+1) - (A-1)*CosOmega - 2*sqrt(A)*alpha ) * gainLinear;
          break;
      case HIGH_SHELF:
    	  A = sqrt(pow(10, point.Boost / 20));
    	  S = sqrt(A + A);
    	  alpha = SinOmega/2 * sqrt((A + 1/A)*(1/S - 1) + 2);

    	  A0 =       (A+1) - (A-1)*CosOmega + 2*sqrt(A)*alpha;
    	  A1 =    2*((A-1) - (A+1)*CosOmega );
    	  A2 =       (A+1) - (A-1)*CosOmega - 2*sqrt(A)*alpha;
    	  B0 =    A*((A+1) + (A-1)*CosOmega + 2*sqrt(A)*alpha ) * gainLinear;
    	  B1 = -2*A*((A-1) + (A+1)*CosOmega ) * gainLinear;
    	  B2 =    A*((A+1) + (A-1)*CosOmega - 2*sqrt(A)*alpha ) * gainLinear;
          break;*/
      case BANDPASS:
    	  alpha = SinOmega * sinh( log(2)/2 * Q * omega/SinOmega);
    	  A0 =   1 + alpha;
    	  A1 =  -2*CosOmega;
    	  A2 =   1 - alpha;
    	  B0 =   alpha * gainLinear;
    	  B1 =   0;
    	  B2 =  -alpha * gainLinear;
          break;
      case BANDSTOP:
    	  alpha = SinOmega * sinh( log(2)/2 * Q * omega/SinOmega);
    	  A0 =   1 + alpha;
    	  A1 =  -2*CosOmega;
    	  A2 =   1 - alpha;
    	  B0 =   1 * gainLinear;
    	  B1 =  -2*CosOmega * gainLinear;
    	  B2 =   1 * gainLinear;
          break;
      case LOWPASS:
  	      alpha = SinOmega / (2 * Q) ;
          A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
          B0 =  (1 - CosOmega)* gainLinear / 2 / A0;
          B1 =  (1 - CosOmega* gainLinear)/ A0;
          B2 =  (1 - CosOmega)* gainLinear / 2 / A0;
          break;
      case HIGHPASS:
    	  alpha = SinOmega / (2 * Q);
          A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
          B0 =  (1 + CosOmega)* gainLinear/ 2/ A0;
          B1 = -(1 + CosOmega)* gainLinear/ A0;
          B2 =  (1 + CosOmega)* gainLinear/ 2/ A0;
          break;
      case BUTTERWORTH_LP:
    	  alpha = SinOmega / (2.0 * 1/sqrt(2));
       	  A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
    	  B0 =  (1 - CosOmega)* gainLinear / 2/ A0;
    	  B1 =  (1 - CosOmega* gainLinear) / A0;
    	  B2 =  (1 - CosOmega)* gainLinear / 2/ A0;
          break;
      case BUTTERWORTH_HP:
    	  alpha = SinOmega / (2.0 * 1/sqrt(2));
    	  A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
    	  B0 = (-(1 + CosOmega)* gainLinear /2 / A0)*(-1);
    	  B1 = -(1 + CosOmega* gainLinear)/ A0;
    	  B2 = (-(1 + CosOmega)* gainLinear /2 / A0)*(-1);
    	  break;
      case BESSEL_LP:
    	  alpha = SinOmega / (2.0 * 1/sqrt(3));
    	  A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
    	  B0 =  (1 - CosOmega)* gainLinear / 2/ A0;
    	  B1 =  (1 - CosOmega* gainLinear)/ A0;
    	  B2 =  (1 - CosOmega)* gainLinear / 2/ A0;
    	  break;
      case BESSEL_HP:
    	  alpha = SinOmega / (2.0 * 1/sqrt(3));
    	  A0 =   1 + alpha;
          A1 =  (-2 * CosOmega/ A0)*(-1);
          A2 =  ((1 - alpha) / A0)*(-1);
    	  B0 = (-(1 + CosOmega)* gainLinear /2 / A0)*(-1);
    	  B1 = -(1 + CosOmega* gainLinear)/ A0;
    	  B2 = (-(1 + CosOmega)* gainLinear /2 / A0)*(-1);
    	  break;
      case LOWPASS_FIRST:
    	  A1 =  pow( 2.7, -omega );
    	  A2 =  0;
    	  B0 =  gainLinear * (1.0 - A1);
    	  B1 =  0;
    	  B2 =  0;
    	  break;
      case HIGHPASS_FIRST:
    	  A1 =  pow( 2.7, -omega );
    	  A2 =  0;
    	  B0 =  gainLinear * A1;
    	  B1 =  -A1 * gainLinear;
    	  B2 =  0;
    	  break;
      default: break;
	}

	DSP_set5PointInDataArray( B2, B1, B0, A2, A1, result );
}

void DSP_LoudnessLevelCalculation(uint8_t source){

	uint8_t result[DATA_LENGTH_20];
	DSP_set5PointInDataArray((0.0196439*dDSP.Loudness[source].LevelLP+0.5230970),(0.0196439*dDSP.Loudness[source].LevelHP+0.5230970), 0, 0, 0, result);
	Transmit_DSP_SafeLoad( (uint8_t *)&result[0], 8, dDSP.Loudness[source].startAddess+6 );
}

void DSP_LoudnessFreqCalculation(uint8_t source){

	uint8_t resultHiA1[4];
	uint8_t resultHiB1[4];
	uint8_t resultHiB0[4];
	uint8_t resultLowA1[4];
	uint8_t resultLowB1[4];
	uint8_t resultLowB0[4];

	float HiA1;
	float HiB1;
	float HiB0;
	float LowA1;
	float LowB1;
	float LowB0;

	HiA1  = (1.000000085 * pow(0.9998691091, dDSP.Loudness[source].FreqHP));
	HiB1  = (1.000000085 * pow(0.9998691091, dDSP.Loudness[source].FreqHP))*(-1);
	HiB0  = (1.000000085 * pow(0.9998691091, dDSP.Loudness[source].FreqHP));

	LowA1 = ((-0.0001304) * dDSP.Loudness[source].FreqLP + 0.9999951);
	LowB1 = 0;
	LowB0 = ((-0.000000009) * pow(dDSP.Loudness[source].FreqLP, 2) + 0.000130899 * dDSP.Loudness[source].FreqLP + 0.000000004);

	DSP_setFloatPointInDataArray( HiA1, resultHiA1 );
	DSP_setFloatPointInDataArray( HiB1, resultHiB1 );
	DSP_setFloatPointInDataArray( HiB0, resultHiB0 );
	DSP_setFloatPointInDataArray( LowA1, resultLowA1 );
	DSP_setFloatPointInDataArray( LowB1, resultLowB1 );
	DSP_setFloatPointInDataArray( LowB0, resultLowB0 );

	Transmit_DSP_SafeLoad( (uint8_t *)&resultHiA1, 4, dDSP.Loudness[source].startAddess );
	Transmit_DSP_SafeLoad( (uint8_t *)&resultHiB1, 4, (dDSP.Loudness[source].startAddess)+1 );
	Transmit_DSP_SafeLoad( (uint8_t *)&resultHiB0, 4, (dDSP.Loudness[source].startAddess)+2 );
	Transmit_DSP_SafeLoad( (uint8_t *)&resultLowA1, 4, (dDSP.Loudness[source].startAddess)+3 );
	Transmit_DSP_SafeLoad( (uint8_t *)&resultLowB1, 4, (dDSP.Loudness[source].startAddess)+4 );
	Transmit_DSP_SafeLoad( (uint8_t *)&resultLowB0, 4, (dDSP.Loudness[source].startAddess)+5 );
}

void DSP_GainCalculation(float data, uint8_t* result ){

	uint8_t   VOLUM_ON    [4]  = {0x01, 0x00, 0x00, 0x00 };

	if(data==OFF){
		for ( short i = 0; i<4; i++){
		   result[i]= VOLUM_ON[i];
		}
	}else{
		DSP_setFloatPointInDataArray( exp(0.1151292546*(data-0.000001)), result );
	}
}

void DSP_VolCalculation(int16_t data, uint8_t* result ){

	float correction = 0;
	uint8_t  VOLUM_OFF   [4]  = {0x00, 0x00, 0x00, 0x01 };

	if(data < 20){
		correction = -0.002;
	}else if(data > 40 && data!= VOL_MAX){
		correction = 0.005;
	}

	if(data==OFF){
		for ( short i = 0; i<4; i++){
		   result[i]= VOLUM_OFF[i];
		}
	}else{
		if(dDSP.Settings[VolumeControlMode].Data == VolumControlOnDSP){ //DigitalDSP control
			DSP_setFloatPointInDataArray( pow(0.9,(VOL_MAX-data))+correction, result );
		}else{//Digital Loudness DSP control
			DSP_setFloatPointInDataArray( pow(0.9,(VOL_MAX-data))+correction, result );
		}
	}
}

void DSP_DelayCalculation( uint16_t data, uint8_t* result ){
	float dataDelay= (data*CONV_FLOAT)*(dDSP.SAMPLE_RATE/1000);
	UdspData.val = (uint16_t)dataDelay;
	for ( int8_t i = 3; i>=0; i--){
	   result[3-i]= UdspData.bytes[i];
	}
}

void DSP_WideStereoCalculation( uint8_t data, uint8_t* result ){
	     if(data == 20){	result[0] = 1;   result[1] = 0;     result[2] = 0;    result[3] = 0;	} //1
	else if(data == 19){	result[0] = 0;   result[1] = 228;   result[2] = 48;   result[3] = 5;	} //0.891250908374786
	else if(data == 18){	result[0] = 0;   result[1] = 203;   result[2] = 89;   result[3] = 24;	} //0.794328212738037
	else if(data == 17){	result[0] = 0;   result[1] = 181;   result[2] = 59;   result[3] = 239;	} //0.707945764064789
	else if(data == 16){	result[0] = 0;   result[1] = 161;   result[2] = 134;  result[3] = 108;	} //0.630957365036011
	else if(data == 15){	result[0] = 0;   result[1] = 143;   result[2] = 245;  result[3] = 154;	} //0.562341332435608
	else if(data == 14){	result[0] = 0;   result[1] = 128;   result[2] = 77;   result[3] = 206;	} //0.501187205314636
	else if(data == 13){	result[0] = 0;   result[1] = 114;   result[2] = 89;   result[3] = 219;	} //0.446683585643768
	else if(data == 12){	result[0] = 0;   result[1] = 101;   result[2] = 234;  result[3] = 90;	} //0.398107171058655
	else if(data == 11){	result[0] = 0;   result[1] = 90;    result[2] = 213;  result[3] = 13;	} //0.354813396930695
	else if(data == 10){	result[0] = 0;   result[1] = 80;    result[2] = 244;  result[3] = 78;	} //0.316227793693542
	else if(data == 9 ){	result[0] = 0;   result[1] = 72;    result[2] = 38;   result[3] = 142;	} //0.281838297843933
	else if(data == 8 ){	result[0] = 0;   result[1] = 64;    result[2] = 77;   result[3] = 230;	} //0.251188635826111
	else if(data == 7 ){	result[0] = 0;   result[1] = 57;    result[2] = 79;   result[3] = 175;	} //0.223872125148773
	else if(data == 6 ){	result[0] = 0;   result[1] = 51;    result[2] = 20;   result[3] = 39;	} //0.199526250362396
	else if(data == 5 ){	result[0] = 0;   result[1] = 45;    result[2] = 134;  result[3] = 34;	} //0.177827954292297
	else if(data == 4 ){	result[0] = 0;   result[1] = 40;    result[2] = 146;  result[3] = 194;	} //0.158489346504211
	else if(data == 3 ){	result[0] = 0;   result[1] = 36;    result[2] = 41;   result[3] = 53;	} //0.141253769397736
	else if(data == 2 ){	result[0] = 0;   result[1] = 32;    result[2] = 58;   result[3] = 126;	} //0.125892519950867
	else if(data == 1 ){	result[0] = 0;   result[1] = 28;    result[2] = 185;  result[3] = 67;	} //0.112201869487762
	else if(data == 0 ){	result[0] = 0;   result[1] = 25;    result[2] = 153;  result[3] = 154;	} //0.100000023841858
}

void DAC_VolCalculation( short data, uint8_t* result ){
	switch(Dixom.Module.AudioDAC.DAC_Type){
    case DAC_ADAU1962    :  UdacData.val = 255 - log(data)/log(1.016185);   break;
    case DAC_AK4458VN    :  UdacData.val = log(data)/log(1.016185);         break;
    case DAC_TAS3251     :  UdacData.val = 255 - log(data)/log(1.016185);   break;
    case DAC_PCM5102A    :                                                  break;
    case DAC_PCM1681     :                                                  break;
    case DAC_AK4440      :  /*NOT SUPPORT*/                                 break;
    case DAC_PCM1690     :  UdacData.val = log(data)/log(1.016185);         break;
	}
	result[0] = UdacData.bytes[0];
}

void DSP_PhatStereoCalculation(short Freq, uint8_t* result ){

	DSP_setFloatPointInDataArray((0.00000000000002513531 * pow(Freq, 3) + 0.00000000189492509739 * pow(Freq, 2) + 0.00006423266794857840 * Freq + 0.00032383357318632733), result );
}


void DSP_LoudnessVolumCalculation(short data){

	uint8_t result [4];
	DSP_VolCalculation(data, (uint8_t *)&result);

	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[0].startAddess+8); //0   LOUDNESSS USB
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[1].startAddess+8); //1   LOUDNESSS BLUETOOTH
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[2].startAddess+8); //2   LOUDNESSS AUX
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[3].startAddess+8); //3   LOUDNESSS RADIO
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[4].startAddess+8); //4   LOUDNESSS SPDIF
	Transmit_DSP_SafeLoad( (uint8_t *)&result, 4,  dDSP.Loudness[5].startAddess+8); //5   LOUDNESSS MIC*/

}







