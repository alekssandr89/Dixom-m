
#ifndef MODULE_DSP_DSP_CALCULATIONS_H_
#define MODULE_DSP_DSP_CALCULATIONS_H_

#include "DSP.h"




void DSP_setFloatPointInDataArray( float val, uint8_t* result );
void DSP_set5PointInDataArray( float val1, float val2, float val3, float val4, float val5, uint8_t* result);
void DSP_CalculateFilter_POINT ( sFiltrParam point, uint8_t* result );
void DSP_LoudnessLevelCalculation(uint8_t source);
void DSP_LoudnessFreqCalculation(uint8_t source);
void DSP_GainCalculation(float data, uint8_t* result );
void DSP_VolCalculation(int16_t data, uint8_t* result );
void DSP_DelayCalculation( uint16_t data, uint8_t* result );
void DSP_WideStereoCalculation( uint8_t data, uint8_t* result );
void DAC_VolCalculation( short data, uint8_t* result );
void DSP_PhatStereoCalculation(short Freq, uint8_t* result );
void DSP_LoudnessVolumCalculation(short data);




#endif /* MODULE_DSP_DSP_CALCULATIONS_H_ */
