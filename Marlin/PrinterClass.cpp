// 
// 
// 

#include "PrinterClass.h"

void PrinterClass::init()
{


}

void PrinterClass::setXPos(char * inStr)
{
	if (strcmp(inStr, _x)!=0 || _pageChanged || !isStarted())
	{
		tX.setText(inStr);
		strcpy(_x, inStr);
	}
}

void PrinterClass::setYPos(char * inStr) 
{
	if (strcmp(inStr, _y) != 0 || _pageChanged || !isStarted())
	{
		tY.setText(inStr);
		strcpy(_y, inStr);
	}
}

void PrinterClass::setZPos(char * inStr)
{
	if (strcmp(inStr, _z) != 0 || _pageChanged || !isStarted())
	{
		tZ.setText(inStr);
		strcpy(_z, inStr);
	}
}

void PrinterClass::setBedTarget(char * inTemp)
{
	if (strcmp(inTemp, _bt) != 0 || _pageChanged || !isStarted())
	{
		tBedT.setText(inTemp);
		strcpy(_bt, inTemp);
	}
}

void PrinterClass::setBedActual(uint8_t inTemp)
{
	uint16_t average = (uint8_t)((_ba + inTemp) / 2);
	if (_ba != average || _pageChanged || !isStarted())
	{
		tBedA.setText(itoa(average, ext, 10));
		_ba = average;
	}
}

void PrinterClass::setExtruderTarget(char * inTemp, int inExtruder)
{
	if (strcmp(inTemp, _et) != 0 || _pageChanged || !isStarted())
	{
		tExtruder1T.setText(inTemp);
		strcpy(_et, inTemp);
	}
}

void PrinterClass::setExtruderActual(uint8_t inTemp, int inExtruder)
{

	uint16_t average = (uint8_t)((_ea + inTemp) / 2);
	if (_ea != average || _pageChanged || !isStarted())
	{
		tExtruder1A.setText(itoa(average, ext, 10));
		_ea = average;

	}
}

void PrinterClass::setFan(char * inSpeed)
{
	if (strcmp(inSpeed, _fan) != 0 || _pageChanged || !isStarted())
	{
		tFan.setText(inSpeed);
		strcpy(_fan, inSpeed);
	}
}

void PrinterClass::setPage(int page)
{
	if (page != _page)
		_pageChanged = true;
	_page = page;

}

int PrinterClass::getPage()
{
	return _page;
}

void PrinterClass::resetPageChanged()
{
	_pageChanged = false;
}

bool PrinterClass::pageChanged()
{
	return _pageChanged;
}

void PrinterClass::setPower(bool status)
{
	if (_power != status || _pageChanged || !isStarted())
	{
		_power = status;

		vaPower.setValue(_power ? "1" : "0");
		if (_page == 2)
			btPower.setValue(_power ? "1" : "0");
		else if (_page == 1)
		{
			if (status)
				pPower.setPic(19);
			else
				pPower.setPic(20);
		}
	}
}

void PrinterClass::setCaseLight(bool status)
{
	if (_caseLight != status || _pageChanged || !isStarted())
	{
		_caseLight = status;

		vaLight.setValue(_caseLight ? "1" : "0");
		if (_page == 2)
			btLight.setValue(_caseLight ? "1" : "0");
		else if (_page == 1)
		{
			//uint32_t val = atoi(subbuff);
			if (status == 1)
				pLight.setPic(18);
			else
				pLight.setPic(20);
		}
	}
}

void PrinterClass::setIsPrinting(bool status)
{
	if (_isPrinting != status || _pageChanged || !isStarted())
	{
		_isPrinting = status;

		if (status == 1)
		{
			pExtruding.setPic(23);
			tmSS.disable();
			vaCounter.setValue(0);
		}
		else
		{
			pExtruding.setPic(20);
			tmSS.enable();
			vaCounter.setValue(0);
		}
	
	}
}

void PrinterClass::setIsHomed(bool status)
{
	if (_isHomed != status || _pageChanged || !isStarted())
	{
		_isHomed = status;
		if (status == 1)
			pHome.setPic(24);
		else
			pHome.setPic(20);
		
	}
}

bool PrinterClass::getIsHomed()
{
	return _isHomed;
}

void PrinterClass::setStarted()
{
	_startup = true;
}

bool PrinterClass::isStarted()
{
	return _startup;
}


void PrinterClass::setMessage(const char * inStr)
{
	tMessage.setText(inStr);
}

void PrinterClass::setTLayers(const char * inStr)
{
	tLayers.setText(inStr);
}
void PrinterClass::setTETA(const char * inStr)
{
	tETA.setText(inStr);
}
void PrinterClass::setTPercentage(const char * inStr)
{
	tPercentage.setText(inStr);
}

//void PrinterClass::setTime(int year, int month, int day, int hour, int minute)
void PrinterClass::setTime()
{
	int myMinute = minute();
	if (myMinute != _minutes)
	{
		_minutes = myMinute;

		int myMonth = month();
		int myDay = day();
		int myHour = hour();


		char myBuff[5];


		strcpy(ext, itoa(year(), myBuff, 10));
		strcat(ext, "-");

		if (myMonth < 10)
			strcat(ext, "0");
		strcat(ext, itoa(myMonth, myBuff, 10));
		
		
		strcat(ext, "-");

		if (myDay < 10)
			strcat(ext, "0");
		strcat(ext, itoa(day(), myBuff, 10));


		strcat(ext, " ");

		if (myHour < 10)
			strcat(ext, "0");
		strcat(ext, itoa(hour(), myBuff, 10));

		strcat(ext, ":");

		if (myMinute < 10)
			strcat(ext, "0");
		strcat(ext, itoa(myMinute, myBuff, 10));

		tTime.setText(ext);


	}

	


}



PrinterClass Printer;

