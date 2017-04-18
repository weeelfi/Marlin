// 
// 
// 

#include "Experimental_LCD.h"
#include "Marlin.h"
#include "twibus.h"
#include "temperature.h"
#include "PrinterClass.h"
#include "queueArray.h"
//#include <TimeLib.h> 




QueueArray inputQueue;
//QueueArray outputQueue;

PrinterClass printer;
char inputBuffer[32];
bool stringEnd = false;
bool stringStart = true;

#define LCD_UPDATE_INTERVAL  500

millis_t next_lcd_update_ms;

void lcd_init() {

	nexInit();
	lcd_update();
	printer.setStarted();

}

void lcd_update() {

	if (!inputQueue.isEmpty()) //process any actual commands from the display
	{
		processBuffer(inputQueue.pop()); //call function to process command from the display
	}

	mySerialEvent(); //general read from serial from display, put commands on queue

	millis_t ms = millis();

	if (ELAPSED(ms, next_lcd_update_ms)) { //time to send update to display
		char temp[10];

		printer.setExtruderActual((uint8_t)thermalManager.degHotend(1), 1);
		printer.setExtruderTarget(itostr3left(thermalManager.degTargetHotend(1)), 1);

		printer.setBedActual((uint8_t)thermalManager.degBed());
		printer.setBedTarget(itoa(thermalManager.degTargetBed(), temp, 10));

		printer.setXPos(ftostr52sp(current_position[X_AXIS]));
		printer.setYPos(ftostr52sp(current_position[Y_AXIS]));
		printer.setZPos(ftostr52sp(current_position[Z_AXIS]));

		printer.setFan(itoa(map(fanSpeeds[0], 0, 255, 0, 100), temp, 10));

		printer.setPower(digitalRead(PS_ON_PIN));
		printer.setCaseLight(digitalRead(CASE_LIGHT_PIN));

		printer.setIsPrinting(planner.movesplanned());
		printer.setIsHomed(axis_homed[X_AXIS] && axis_homed[Y_AXIS] && axis_homed[Z_AXIS] ? true : false);

		printer.resetPageChanged();

		//printer.setTime(year(), month(), day(), hour(), minute());
		printer.setTime();

		next_lcd_update_ms = ms + LCD_UPDATE_INTERVAL;
	}


}

void processBuffer(const char* receivedString) {

	int receivedByte = strlen(receivedString);
	int strLength = 0;
	char subbuff[32] = { 0 };

	switch (receivedString[0]) {
	case 'E':
		strLength = receivedByte - 3;
		memcpy(subbuff, &receivedString[3], strLength);
		subbuff[strLength + 1] = '\0';

		thermalManager.setTargetHotend(atoi(subbuff), ((int)receivedString[1]) - 1);
		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		SERIAL_EOL;

		break;
	case 'A':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength + 1] = '\0';

		thermalManager.setTargetBed(atoi(subbuff));
		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		SERIAL_EOL;

		break;
	case 'F':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength + 1] = '\0';

		fanSpeeds[0] = map(atoi(subbuff), 0, 100, 0, 255); //(ceil(atoi(subbuff) * 2.54));
		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", atoi(subbuff));
		//SERIAL_ECHOLNPAIR("subbuff: ", subbuff);
		//SERIAL_ECHOLNPAIR("receivedByte: ", receivedByte);
		//SERIAL_ECHOLNPAIR("strLength: ", strLength);
		SERIAL_EOL;

		break;
	case 'G':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[strLength] = '\0';

		enqueue_and_echo_command(subbuff);
		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		SERIAL_EOL;

		break;
	case 'P':
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';

		printer.setPage(atoi(subbuff));

		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		SERIAL_EOL;

		break;
	case 'I': //Power status
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';

		printer.setPower((bool)atoi(subbuff));

		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		//SERIAL_ECHOLNPAIR("subbuff: ", subbuff);
		SERIAL_EOL;

		break;
	case 'C': //Case Light
		strLength = receivedByte - 2;
		memcpy(subbuff, &receivedString[2], strLength);
		subbuff[receivedByte - strLength] = '\0';

		printer.setCaseLight((bool)atoi(subbuff));

		SERIAL_ECHO_START;
		SERIAL_ECHOLNPAIR("receivedString: ", receivedString);
		//SERIAL_ECHOLNPAIR("subbuff: ", subbuff);
		SERIAL_EOL;

		break;

	}
}


void mySerialEvent() {
	volatile char inputChar;
	volatile uint8_t stringLength = 0;
	stringStart = false;
	stringEnd = false;
	memset(inputBuffer, 0, 32);
	while (nexSerial.available())
	{
		inputChar = (char)nexSerial.read();
		delayMicroseconds(200);
		if ((int)inputChar >= 32 && (int)inputChar < 255)
		{
			if (inputChar != '!')
			{
				stringEnd = false;
			}
			if (!stringEnd && inputChar != '!' && inputChar != '*')
			{
				inputBuffer[stringLength] = inputChar;
				stringLength++;
			}

			if (inputChar == '!' && !stringEnd)
			{
				stringEnd = true;
				inputBuffer[stringLength] = 0;

				if (inputBuffer[0] == 'P')
				{
					ProcessPage(inputBuffer, stringLength);
					stringLength = 0;
				}
				else
				{
					inputQueue.push(inputBuffer);
					stringLength = 0;
				}
			}
		}

	}
}

void ProcessPage(char * inputString, uint8_t receivedBytes)
{
	char subbuff[4];
	uint8_t strLength = receivedBytes - 2;

	memcpy(subbuff, &inputString[2], strLength);
	subbuff[strLength] = 0;

	printer.setPage(atoi(subbuff));

	SERIAL_ECHO_START;
	SERIAL_ECHOLNPAIR("Page: ", printer.getPage());
	SERIAL_EOL;

}

void lcd_setstatus(const char* message, const bool persist) {
	processMessage(message);
}



void lcd_setstatuspgm(const char* message, uint8_t level) {
	processMessage(message);
}

void processMessage(const char * message)
{

	SERIAL_ECHO_START;
	SERIAL_ECHOLNPAIR("Message: ", message);
	SERIAL_EOL;
	char subbuff[32];
	char buff[5];

	if (message[0] == 'L' && message[1] == 'a') //La/yer
	{
		//SERIAL_ECHO_START;
		//SERIAL_ECHOLN("Layer Start");
		//SERIAL_EOL;
		const uint8_t startPos = 6;
		uint8_t toCounter = startPos;
		while (toCounter<32)
		{
			if (message[toCounter] == ' ')
			{
				toCounter;
				memcpy(subbuff, &message[startPos], toCounter - startPos);
				subbuff[toCounter - startPos] = '\0';
				break;
			}

			toCounter++;
		}
		printer.setTLayers(subbuff);
		//SERIAL_ECHO_START;
		//SERIAL_ECHOLN("Layer End");
		//SERIAL_ECHOLNPAIR("Layer: ",subbuff);
		//SERIAL_EOL;

		return;

	}
	else if (message[0] == 'E' && message[1] == 'T' && message[2] == 'L')
	{
		//SERIAL_ECHO_START;
		//SERIAL_ECHOLN("ETA");
		//SERIAL_EOL;
		strlcpy(subbuff, &message[4], 11);
		printer.setTETA(subbuff);
	}
	else if (message[0] == 'T' && message[1] == 'M' && message[2] == '=')
	{
		unsigned long myTime;
		strlcpy(subbuff, &message[3], 11);
		myTime = atol(subbuff) + 7200;
		setTime(myTime);

		//SERIAL_ECHO_START;
		//SERIAL_ECHOLNPAIR("myTime: ", subbuff);
		//SERIAL_ECHOLNPAIR("Day: ", day());
		//SERIAL_ECHOLNPAIR("Month: ", month());
		//SERIAL_ECHOLNPAIR("Year: ", year());
		//SERIAL_EOL;

	}
	else
	{
		bool dotFound = false;
		uint8_t dotLocation = 0;
		if (message[0] != 'I' && message[1] != 'P' && message[0]!='H') //exclude IP Address and Heating...
		{
			//SERIAL_ECHO_START;
			//SERIAL_ECHOLN("Percentage");
			//SERIAL_EOL;
			for (uint8_t i = 0; i < 9; i++)
			{
				if (message[i] == '.')
				{
					dotFound = true;
					dotLocation = i;
				}
			}
		}

		if (dotFound)
		{
			memcpy(subbuff, &message[0], dotLocation);
			subbuff[dotLocation] = '%';
			subbuff[dotLocation + 1] = '\0';
			printer.setTPercentage(subbuff);
			return;
		}
		



		printer.setMessage(message);
		return;


	}

}