#ifndef __MAIN_MENU_H__
#define __MAIN_MENU_H__

void      main_Menu(uint8_t *Received_String);
void      GENERAL(uint8_t ControlByte, uint8_t Function, uint8_t *Received_String);
void      SHOW_ALL_MODIFIED_PARAMETERS();
void      RECIVE_DATA_LOSS_TEST (uint8_t *InputData);


#endif

