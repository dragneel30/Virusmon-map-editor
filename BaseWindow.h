
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ToolsArea.h"
#include "Vector2.h"
#include "EditorArea.h"

class BaseWindow : public DocumentWindow
{	
public:
	BaseWindow(String name, Vector2i size)
		: DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons)
	{
		setSize(size.x, size.y);
		centreWithSize(size.x, size.y);
		setUsingNativeTitleBar(false);
		setResizable(true, true);
		setVisible(true);
	}


	
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseWindow)
};


/*
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
*/
