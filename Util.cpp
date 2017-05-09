

#include "Util.h"
void myLog(const std::string& str)
{
	Logger::outputDebugString(str);
}

void myLog(int strInteger)
{
	myLog(std::to_string(strInteger));
}

void myLog(float strFloat)
{
	myLog(std::to_string(strFloat));
}


int makePOT(int n)
{
	int temp = 2;
	while (temp <= n)
	{
		temp *= 2;
	}
	return temp / 2;
}

