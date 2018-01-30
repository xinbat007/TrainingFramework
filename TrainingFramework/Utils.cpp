#include <stdafx.h>
#include "Utils.h"
#include <time.h>
///////////////////////////////////////////////////
Utils* Utils::ms_pInstance = NULL;
///////////////////////////////////////////////////
unsigned long Utils::GetTimeMilisecond()
{
#ifdef WIN32
	return GetTickCount();
#else
	time_t _curTime;
	_curTime = time(0);
	unsigned long curTime = (unsigned long)_curTime;
	//LOGI("AAAA: time: %d", curTime);
	
	/*
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
    //double l = 1000.0 * ts.tv_sec + (double) ts.tv_nsec / 1000000.0;
	// Return time in micro seconds
	long l = static_cast<long> ((double)(ts.tv_sec) * 1000000.0 + (double)ts.tv_nsec / 1000.0);
	double d = double(l) / double(1000000);	// GetSecondsDouble
	long l2 = (long)(d * 1000 * 1000);	// GetMicroseconds
	unsigned long l3 = (unsigned long)(l2 / 1000);	// getRealTime()
	*/
	return curTime;
#endif
}
///////////////////////////////////////////////////
/*std::string Utils::FindSubStringAtIndex(std::string sInput, int index, const char* cSplit)
{
	int i = 1;
	std::string s1, s2;
	std::size_t posSemicolon;
	posSemicolon = sInput.find(cSplit);
	s1 = sInput.substr(0, posSemicolon);
	s2 = sInput.substr(posSemicolon + 1);
	if (index == 0)
		return s1;
	while (i < index)
	{
		posSemicolon = s2.find(cSplit);
		s2 = s2.substr(posSemicolon + 1);
		i++;
	}
	posSemicolon = s2.find(cSplit);
	s2 = s2.substr(0, posSemicolon);
	return s2;
}*/
///////////////////////////////////////////////////
///////////////////////////////////////////////////