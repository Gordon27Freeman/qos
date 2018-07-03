#include <i686/time.h>
#include <i686/pio.h>
using namespace Time;

#define CURRENT_YEAR 2018

int centuryRegister = 0x00;

unsigned int second;
unsigned int minute;
unsigned int hour;
unsigned int day;
unsigned int month;
unsigned int year;

enum {
	cmosAddress = 0x70,
	cmosData    = 0x71
};

static int GetUIPF()
{
	outb(cmosAddress, 0x0A);
	return (inb(cmosData) & 0x80);
}

static unsigned char GetRTCReg(int reg)
{
	outb(cmosAddress, reg);
	return inb(cmosData);
}

static void ReadRTC()
{
	unsigned char century = 0;
	unsigned char lastSecond;
	unsigned char lastMinute;
	unsigned char lastHour;
	unsigned char lastDay;
	unsigned char lastMonth;
	unsigned char lastYear;
	unsigned char lastCentury;
	unsigned char registerB;

	while (GetUIPF());
	second = GetRTCReg(0x00);
	minute = GetRTCReg(0x02);
	hour = GetRTCReg(0x04);
	day = GetRTCReg(0x07);
	month = GetRTCReg(0x08);
	year = GetRTCReg(0x09);
	if(centuryRegister != 0) century = GetRTCReg(centuryRegister);
 
	do {
		lastSecond = second;
		lastMinute = minute;
		lastHour = hour;
		lastDay = day;
		lastMonth = month;
		lastYear = year;
		lastCentury = century;
 
		while (GetUIPF());
		second = GetRTCReg(0x00);
		minute = GetRTCReg(0x02);
		hour = GetRTCReg(0x04);
		day = GetRTCReg(0x07);
		month = GetRTCReg(0x08);
		year = GetRTCReg(0x09);
		if(centuryRegister != 0) century = GetRTCReg(centuryRegister);
	} while( (lastSecond != second) || (lastMinute != minute) || (lastHour != hour) ||
		   (lastDay != day) || (lastMonth != month) || (lastYear != year) ||
		   (lastCentury != century) );
 
	registerB = GetRTCReg(0x0B);

	if (!(registerB & 0x04))
	{
		second = (second & 0x0F) + ((second / 16) * 10);
		minute = (minute & 0x0F) + ((minute / 16) * 10);
		hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
		day = (day & 0x0F) + ((day / 16) * 10);
		month = (month & 0x0F) + ((month / 16) * 10);
		year = (year & 0x0F) + ((year / 16) * 10);
		if(centuryRegister != 0) century = (century & 0x0F) + ((century / 16) * 10);
	}

	if (!(registerB & 0x02) && (hour & 0x80)) hour = ((hour & 0x7F) + 12) % 24;

	if(centuryRegister != 0) year += century * 100;
	else
	{
		year += (CURRENT_YEAR / 100) * 100;
		if(year < CURRENT_YEAR) year += 100;
	}
}

void Time::GetTime(unsigned char *h, unsigned char *m, unsigned char *s)
{
	ReadRTC();
	*h = hour;
	*m = minute;
	*s = second;
}

void Time::GetDate(unsigned char *d, unsigned char *m, unsigned char *y)
{
	ReadRTC();
	*d = day;
	*m = month;
	*y = year&0xf;
}
