/*
  ==============================================================================

    Tileset.h
    Created: 18 May 2017 5:49:02am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class Tileset    : public Component
{
public:
    Tileset();
    ~Tileset();

    void paint (Graphics&) override;
    void resized() override;


	OwnedArray<
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Tileset)
};
