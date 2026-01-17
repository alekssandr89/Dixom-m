#include "Action_displayMenu.h"


void DisplayAction_SourceSetup(sDisplayMenu *DisplayMenu) {

	if (DisplayMenu->MenuStatus == Status_Setup) {

		if (DisplayMenu->MenuAction == Action_Up) {
			DSP_SoursRouting(1);
		} else if (DisplayMenu->MenuAction == Action_Down) {
			DSP_SoursRouting(0);
		}
	}
}

void DisplayAction_LcdBrightness(sDisplayMenu *DisplayMenu) {

	if (DisplayMenu->MenuStatus == Status_Setup) {

		if (DisplayMenu->MenuAction == Action_Up) {
			SetDisplayBrightness(0);
		} else if (DisplayMenu->MenuAction == Action_Down) {
			SetDisplayBrightness(1);
		}
	}
}

void DisplayAction_GroupVol(sDisplayMenu *DisplayMenu) {

	if (DisplayMenu->MenuStatus == Status_Setup) {

		uint8_t ch = 0;

		switch (DisplayMenu->MenuLevel_3) {
			case 2:
				ch = 29;
				break;
			case 3:
				ch = 32;
				break;
			case 4:
				ch = 35;
				break;
			case 5:
				ch = 38;
				break;
			case 6:
				ch = 39;
				break;
			case 7:
				ch = 44;
				break;
			case 8:
				ch = 47;
				break;
		}

		if (DisplayMenu->MenuAction == Action_Up) {
			DSP_Action_ControlVolum  (0, ch, 0, 0, 0, YES);
		} else if (DisplayMenu->MenuAction == Action_Down) {
			DSP_Action_ControlVolum  (1, ch, 0, 0, 0, YES);
		}
	}
}

