
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToolsArea.h"
#include "Vector2.h"
#include "EditorArea.h"
#include "PropertiesArea.h"


class BaseWindow : public Component
{
public:
    BaseWindow(Vector2i size);
    ~BaseWindow();

    void paint (Graphics&) override;
    void resized() override;

private:

	ToolsArea toolsArea;
	EditorArea editorArea;
	PropertiesArea propertiesArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BaseWindow)
};
