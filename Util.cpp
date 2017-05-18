

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

File getAbsolutePath(String relativePath)
{
	//myLog(File::getCurrentWorkingDirectory().getChildFile(relativePath).getFullPathName().toStdString());
	return File::getCurrentWorkingDirectory().getChildFile(relativePath);
}

Image getImageFromFile(String relativePath)
{
	return ImageFileFormat::loadFrom(getAbsolutePath(relativePath));
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

