
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TabbedComponentWrapper.h"

class EditorArea : public Component
{
public:
    EditorArea();
    ~EditorArea();

    void paint (Graphics&) override;
    void resized() override;

private:
	TabbedComponentWrapper tab;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorArea)
};
