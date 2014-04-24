#include "DayClock.h"


DayClock::DayClock(int startingHour)
{
	msecTime = startingHour * HOURTIME;
	speed = 1.0f;
}


DayClock::~DayClock(void)
{
}


void DayClock::Update(float msec)
{
	msecTime += msec * speed;

	if(msecTime > FULLDAYTIME)
	{
		msecTime = 0;
	}

	hour = CalcHour();
	minute = CalcMinute();
	second = CalcSecond();

}

float DayClock::GetTime01()
{
	return msecTime/FULLDAYTIME;
}

int DayClock::CalcHour()
{
	float hour = msecTime/HOURTIME;

	return (int)hour;
}


int DayClock::GetHour()
{
	return hour;


}

int DayClock::CalcMinute()
{
	float hour = msecTime/ HOURTIME;
	
	float minute = hour - (int)hour;
	
	minute = (minute * 60);

	return (int)minute;

}

int DayClock::GetMinute()
{
	return minute;
}

void DayClock::HigherSpeed() {
	speed+=0.1f;
}

void DayClock::LowerSpeed() {
	if(speed>0.0f)
		speed-=0.1f;
}


int DayClock::CalcSecond()
{
	float minute = msecTime/ (HOURTIME/60);
	float second = minute - (int)minute;

	second = second* 60;

	return second;
}

int DayClock::GetSecond()
{
	return second;
}