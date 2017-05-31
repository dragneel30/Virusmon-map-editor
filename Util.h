
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vector2.h"
#include <string>
#include <vector>
void myLog(const std::string& str);
void myLog(int strInteger);
void myLog(float strFloat);
void myLog(wchar_t character);


File getAbsolutePath(String relativePath);
Image getImageFromFile(String relativePath);

int makePOT(int n);



