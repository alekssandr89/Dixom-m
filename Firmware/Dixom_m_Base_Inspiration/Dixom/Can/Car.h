/*
 * Car.h
 *
 *  Created on: 10 ���. 2022 �.
 *      Author: 79825
 */

#ifndef CAN_CAR_H_
#define CAN_CAR_H_

void UpdateCarFunctionPointers();

void Button_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
void Climate_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
void Dashboard_skoda_octavia_A7(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);

void Button_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
void Climate_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);
void Dashboard_audi_A4_B6(uint8_t msgType, uint8_t dlc, uint32_t id, uint8_t *data);

uint16_t Add_can_action_queue(uint16_t ButtonID, uint16_t Pressed);

typedef enum
{
	NO_MANUFACTURER   =0,
	AUDI              =1,
	SKODA             =2,

	/*BMW               =2,
	FORD              =3,
	HONDA             =4,
	HYNDAY            =5,
	KIA               =6,
	LADA              =7,
	MAZDA             =8,
	MERSEDES_BENZ     =9,
	MITSUBISHI        =10,
	NISSAN            =11,
	RENAULT           =12,

	TOYOTA            =14,
	VOLTSWAGEN        =15,*/
}tManufacturer;

typedef enum
{
	year1990 = 0,
	year1991 = 1,
	year1992 = 2,
	year1993 = 3,
	year1994 = 4,
	year1995 = 5,
	year1996 = 6,
	year1997 = 7,
	year1998 = 8,
	year1999 = 9,
	year2000 = 10,
	year2001 = 11,
	year2002 = 12,
	year2003 = 13,
	year2004 = 14,
	year2005 = 15,
	year2006 = 16,
	year2007 = 17,
	year2008 = 18,
	year2009 = 19,
	year2010 = 20,
	year2011 = 21,
	year2012 = 22,
	year2013 = 23,
	year2014 = 24,
	year2015 = 25,
	year2016 = 26,
	year2017 = 27,
	year2018 = 28,
	year2019 = 29,
	year2020 = 30,
	year2021 = 31,
	year2022 = 32
}tCarYear;


typedef enum
{
	OCTAVIA           =0,
	RAPID             =1,
	SUPERB            =2,
	KAROQ             =3,
	YERI              =4,
	FAIA              =5,
	KODIAQ            =6,
}tModelSkoda;

typedef enum
{
	A4                =0,
}tModelAudi;

typedef enum
{
	/*FIRST_A1_1996_2004      =0,
	RESTYLING_2000_2010     =1,
	SECOND_A5_2004_2010     =2,
	RESTYLING_FL_2008_2012  =3,*/
	THIRD_A7_2013           = 0,
	//RESTYLING_2017          =5,
}tGenerationSkodaOctavia;

typedef enum
{
	//FIRST_1995_2001         =0,
	SECOND_B6_2000_2006     = 0,
	/*THIRD_B7_2004_2008      =2,
	FOURTH_B8_2008_2015     =3,
	FIFTH_B9_2015_2020      =4,*/
}tGenerationAudiA4;

typedef enum
{
	MANUAL                  =0,
	AUTOMATIC               =1,
	ROBOTIC                 =2,
	VARIBLE                 =3,
}tTransmission;

typedef enum
{
	FRONT                  =0,
	REAR                   =1,
	FULL                   =2,
}tDriveUnit;

typedef enum
{
	LEFT                    =0,
	RIGHT                   =4,
}tHandlebarSide;




#define NUM_ELEM_BUTTON_SKODA_OCTAVIA_A7  14
#define NUM_ELEM_DASHBOARD_SKODA_OCTAVIA_A7  2

static const uint32_t SKODA_OCTAVIA_A7_ID_CAN[2]={
	0x5BF, //Button
	0x3DB  //Rear
};


static const uint32_t AUDI_A4_B6[2]={
	0x5C3, //Button
	0x351  //Rear
};


static const uint8_t SKODA_OCTAVIA_A7_BUTTON[NUM_ELEM_BUTTON_SKODA_OCTAVIA_A7*10] = {
  //dlc 1   D1    D2    D3    D4    D5    D6    D7    D8
     4,    0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x00, 0,   // 0 no preset
     4,    0x15, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 10,  // 1 media next
     4,    0x16, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 20,  // 3 media previus
     4,    0x13, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 30,  // 4 media encoder
     4,    0x12, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 40,  // 5 media encoder up
     4,    0x12, 0x00, 0x0F, 0x13, 0x00, 0x00, 0x00, 0x00, 50,  // 6 media encoder down
     4,    0x14, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 60,  // 7 media source
     4,    0x07, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 70,  // 8 panel encoder
     4,    0x06, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 80,  // 9 panel encoder up
     4,    0x06, 0x00, 0x0F, 0x13, 0x00, 0x00, 0x00, 0x00, 90,  // 10 panel encoder down
     4,    0x08, 0x00, 0x01, 0x13, 0x00, 0x00, 0x00, 0x00, 100, // 11 panel source
};

static const uint8_t SKODA_OCTAVIA_A7_DASHBOARD[NUM_ELEM_DASHBOARD_SKODA_OCTAVIA_A7*9] = {
  //dlc 1   D1    D2    D3    D4    D5    D6    D7    D8
     8,    0x50, 010,  0x01, 0x02, 0x47, 0x00, 0x01, 0x00, // 12 reverse enabled ?
     8,    0x50, 010,  0x01, 0x00, 0x47, 0x00, 0x01, 0x00, // 13 reverse disabled ?
};


#endif /* CAN_CAR_H_ */
