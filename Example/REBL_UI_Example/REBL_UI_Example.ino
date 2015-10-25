#include "REBL_UI.h"
#include <LiquidCrystal_SPI_8Bit.h>
#include <Time.h>

MenuItem PROGMEM menuItems[] = {
		{"display", fun1 },
		{"press", pressToExit },
		{"enter number", pickNumber },
		{"enter string", fourLetterWord },
		{"set time", enterTime },
		{"show time", showTime }
};

MenuList menuList(menuItems, menuListSize(menuItems));



void setup() {
	initLCD();
	initInterface();
	reblMenu.setCurrentMenu(&menuList);
}

void loop() {
	pollButton();
	static unsigned long preMil = millis();
	unsigned long curMil = millis();
	if (curMil - preMil >= DISPLAY_DELAY) {
		preMil = curMil;
		reblMenu.doMenu();
		doDisplay();
	}

}


boolean fun1() {
	displayLineLeft(0, F("Display This"));
	displayLineLeft(1, F("   ")); // to clear the line
			return displayDelay(3);
		}

boolean pressToExit() {
	displayLineLeft(0, F("Press Button"));
	displayLineLeft(1, F("to Exit"));
	if(!isButtonOn()) {
		buttonOn();
	}
	else if(checkButton()) {
		buttonOff();
		return true;
	}
	return false;
}

boolean pickNumber() {
	static int state = 0;
	static int val = 0;

	switch (state) {
	case 0: {
		val = 0;
		state++;
	}
	case 1: {
		if (enterNumber(val, F("Enter Number"), 0, 50, 1)) {
			state++;
		}
		break;
	}
	case 2: {
		if (displayDelay(3)) {
			state = 0;
			return true;
		}
		displayLineLeft(0, "You Entered:");
		char buf[6];
		sprintf_P(buf, PSTR("%d"), val);
		displayLineLeft(1, buf);
		break;
	}
	}  // end switch (state)
	return false;
}

boolean fourLetterWord() {
	static char theStr[5] = "four";
	static int state = 0;

	switch (state) {

	case 0: {  // enter the string
		if (enterString(theStr, 4)) {
			state++;
			break;
		}
		theStr[4] = 0;  // just in case.
		displayLineLeft(0, F("4 Letter Word"));
		displayLineLeft(1, theStr);
		break;
	}

	case 1: {   // display the entered string
		displayLineLeft(0, F("You Entered"));
		displayLineLeft(1, theStr);
		if(displayDelay(3)) {
			state = 0;
			encoderOff();
			buttonOff();
			return true;
		}
		break;
	}

}		//  end switch (state)
	return false;
}

boolean enterTime() {
	static time_t timeSetTime = 0;
	static int state = 0;

	switch (state) {

	case 0: {  // turn things on and setup variables
		timeSetTime = now();
		state++;
	}
		/* no break */
	case 1: {   // input the time
		if (inputTime(timeSetTime)) {
			state++;
			setTime(timeSetTime);
		}
		break;
	}
	case 2: {   // display the running time for 10 seconds
		displayTime(now());
		if (displayDelay(10)) {
			state = 0;
			return true;
		}
		break;
	}

	}  // end switch (state)
	return false;
}

boolean showTime() {
	displayTime(now());  // display the running time
	return displayDelay(10);  // do it for ten seconds
}
