/*
  ==============================================================================

    Tile.h
    Created: 18 May 2017 5:49:13am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Properties.h"
class Tile : public Component
{
public:
    Tile();
    ~Tile();

    void paint (Graphics&) override;
    void resized() override;

private:
	
	Properties property;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tile)
};
