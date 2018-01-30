#pragma once

#include "Commons.h"

class Utils
{	
protected:
	static Utils* ms_pInstance;
private:
public:
	unsigned long GetTimeMilisecond();
	//std::string FindSubStringAtIndex(std::string sInput, int index, const char* cSplit);
	static Utils* GetInstance()
	{	
		if (ms_pInstance == NULL)
		{
			//printf("CREATE Utils\n");
			ms_pInstance = new Utils();
		}
		return ms_pInstance;
	};
	static void DestroyInstance()
	{
		if (ms_pInstance)
		{
			//printf("DESTROY Utils\n");
			delete(ms_pInstance);
			ms_pInstance = NULL;
		}
	}
};