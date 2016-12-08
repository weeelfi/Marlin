// PrinterClass.h

#ifndef _PRINTERCLASS_h
#define _PRINTERCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Wire.h"
#include "Nextion.h"
#include "TimeLib.h"



class PrinterClass
{
 protected:


 public:
	void init();

	void setXPos(char * inStr);
	void setYPos(char * inStr);
	void setZPos(char * inStr);

	void setBedTarget(char * inTemp);
	void setBedActual(uint8_t inTemp);
	void setExtruderTarget(char * inTemp, int inExtruder);
	void setExtruderActual(uint8_t inTemp, int inExtruder);
	void setFan(char * inSpeed);

	void setPage(int page);
	int getPage();
	void resetPageChanged();
	bool pageChanged();
	
	void setPower(bool status);
	void setCaseLight(bool status);

	void setIsPrinting(bool status);
	void setIsHomed(bool status);
	bool getIsHomed();

	void setStarted();
	bool isStarted();

	void setMessage(const char * inStr);

	void setTLayers(const char * inStr);
	void setTETA(const char * inStr);
	void setTPercentage(const char * inStr);

	//void setTime(int year, int month, int day, int hour, int minute);
	void setTime();
	

private:
	char _x[7], _y[7], _z[7];
	char _et[4], _fan[4], _bt[4];
	uint8_t  _ba, _ea;
	int  _page = 0;
	bool _pageChanged;
	bool _caseLight, _power = 0;
	bool _isPrinting = false;
	bool _isHomed = false;
	bool _startup = false;

	NexText tExtruder1A = NexText(1, 28, "tExtruder1A");
	NexText tExtruder1T = NexText(1, 27, "tExtruder1T");

	NexText tBedT = NexText(1, 6, "tBedT");
	NexText tBedA = NexText(1, 5, "tBedA");

	NexText tX = NexText(1, 16, "tX");
	NexText tY = NexText(1, 17, "tY");
	NexText tZ = NexText(1, 18, "tZ");

	NexText tMessage = NexText(1, 7, "tMessage");

	NexText tFan = NexText(1, 21, "tFan");

	NexText tLayers = NexText(1, 21, "MainPage.tLayers");
	NexText tETA = NexText(1, 19, "MainPage.tETA");
	NexText tPercentage = NexText(1, 8, "MainPage.tPercentage");

	NexDSButton btPower = NexDSButton(2, 5, "MainMenu.btPower");
	NexDSButton btLight = NexDSButton(2, 6, "MainMenu.btLight");

	NexPicture pPower = NexPicture(1, 29, "MainPage.pPower");
	NexPicture pLight = NexPicture(1, 30, "MainPage.pLight");

	NexVariable vaPower = NexVariable(2, 8, "MainMenu.vaPower");
	NexVariable vaLight = NexVariable(2, 9, "MainMenu.vaLight");

	NexPicture pHome = NexPicture(1, 35, "MainPage.pHome");
	NexPicture pExtruding = NexPicture(1, 34, "MainPage.pExtruding");

	NexTimer tmSS = NexTimer(1, 37, "tmSS");
	NexVariable vaCounter = NexVariable(1, 38, "vaCounter");

	NexVariable tTime = NexVariable(1, 39, "MainPage.tTime");

	char ext[10];
	int _minutes;

};

extern PrinterClass Printer;

#endif

