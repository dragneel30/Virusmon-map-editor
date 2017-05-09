

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class PropertiesArea    : public Component
{
public:
    PropertiesArea();
    ~PropertiesArea();

    void paint (Graphics&) override;
    void resized() override;

private:




    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertiesArea)
};
