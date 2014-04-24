#pragma once


//#define REALTIMECLOCK


#ifdef REALTIMECLOCK
	#define FULLDAYTIME 86400000
	#define HOURTIME 3600000
#else
	#define FULLDAYTIME 120000
	#define HOURTIME 5000

#endif 




class DayClock
{
public:
	DayClock(int startingHour);
	~DayClock(void);

	void Update(float msec);


	int GetHour();
	int GetMinute();
	int GetSecond();

	float getSpeed() {return speed; }

	void HigherSpeed();
	void LowerSpeed();
	
	float GetTime01();


	float msecTime;


private:

	float speed;

	int CalcHour();
	int CalcMinute();
	int CalcSecond();

	int hour;
	int minute;
	int second;
	
};

