/*
  ==============================================================================

    Tool.h
    Created: 16 May 2017 8:55:47am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Util.h"
class ToolButton : public ImageButton
{
public:
	 
	ToolButton(const String& name = String()) 
		: ImageButton(name), selected(false)
	{

	}



	void paint(Graphics& g) override
	{
		ImageButton::paint(g);
		if (isSelected())
		{
			g.setColour(colorWhenSelected);
			g.fillRect(0, 0, getWidth(), getHeight());
			repaint();
		}
	}


	bool isSelected() { return selected; }
	void toggle() { selected = !selected; }
private:
	bool selected;
	static const Colour colorWhenSelected;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolButton)
};
