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
		: ImageButton(name), selected(false), hovering(false)
	{

	}

	void mouseEnter(const MouseEvent& event) override
	{
		hovering = true;
	}

	void mouseExit(const MouseEvent& event) override
	{

		hovering = false;
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
		else if (isHovering())
		{
			g.setColour(colorWhenHovered);
			g.drawRect(0, 0, getWidth(), getHeight());
			repaint();
			
		}
	}


	bool isSelected() { return selected; }
	bool isHovering() { return hovering; }
	void toggle() { selected = !selected; }
private:
	bool selected;
	bool hovering;
	static const Colour colorWhenSelected;
	static const Colour colorWhenHovered;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolButton)
};
