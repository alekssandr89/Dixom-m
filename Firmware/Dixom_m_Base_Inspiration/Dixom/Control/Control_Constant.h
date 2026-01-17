/*
 * Control_Constant.h
 *
 *  Created on: 23 ����. 2021 �.
 *      Author: 79825
 */

#ifndef CONTROL_CONTROL_CONSTANT_H_
#define CONTROL_CONTROL_CONSTANT_H_

#define ANLOG_SETTINGS_COUNT  10

#define SettingsAdcInterval              0 // 135
#define Encoder1Type             1 // 145 ����� ���� �������� 1 ������
#define Encoder2Type             2 // 146 ����� ���� �������� 2 ������

static const uint16_t  DEFAULT_CONTROL_SETTINGS[ANLOG_SETTINGS_COUNT] = {
		150, // 0 Adc Interval
		0, // 1  Encoder1Type
		0, // 2  Encoder1Type
		0, // 3  Auto connect 0 no, 1 yes
		0, // 4  Can data out permision
		0, // 5
		0, // 6
		0, // 7
		0, // 8
		0, // 9
};

#endif /* CONTROL_CONTROL_CONSTANT_H_ */
