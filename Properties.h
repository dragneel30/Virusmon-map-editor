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
//==============================================================================
/*
*/
class Properties
{
public:
	Properties()
	{
	
	}

	void add(std::string key, std::string value)
	{
		strProperties.push_back(std::make_pair(key, value));
	}

private:

	std::vector<std::pair<std::string, std::string>> strProperties;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Properties)
};
