/*
  ==============================================================================

    Properties.h
    Created: 18 May 2017 6:04:22am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <vector>
#include "PropertiesWindow.h"
class Properties
{
public:
	Properties()
	{
	
	}

	void add(std::string property, std::string value)
	{
		strProperties.push_back(std::make_pair(property, value));
	}

	const std::vector<std::pair<String, String>>& getProperties()
	{
		return strProperties;
	}
	void show()
	{
		window = new PropertiesWindow("Properties", Vector2i(200, 800), strProperties);
		window->setVisible(true);
	}
private:
	static PropertiesWindow* window;
	std::vector<std::pair<String, String>> strProperties;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Properties)
};
