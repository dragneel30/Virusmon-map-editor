
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Vector2.h"

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
