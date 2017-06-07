/*
  ==============================================================================

    Properties.cpp
    Created: 18 May 2017 6:04:22am
    Author:  Lorence

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Properties.h"


PropertiesWindow *Properties::window = nullptr;
void Properties::show()
{
	if (window != nullptr)
	{
		delete window;
		window = nullptr;
	}
	window = new PropertiesWindow("Properties", Vector2i(200, 800), *this);
	window->setVisible(true);
}