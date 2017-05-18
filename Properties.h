/*
  ==============================================================================

    Properties.h
    Created: 18 May 2017 6:04:22am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Properties    : public Component
{
public:
    Properties();
    ~Properties();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Properties)
};
