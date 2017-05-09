

#include "../JuceLibraryCode/JuceHeader.h"
#include "BaseWindow.h"
#include <string>
BaseWindow::BaseWindow(Vector2i size)
{
	setSize(size.x, size.y);
	addAndMakeVisible(editorArea);
	addAndMakeVisible(toolsArea);
	addAndMakeVisible(propertiesArea);


}

BaseWindow::~BaseWindow()
{

}

void BaseWindow::paint (Graphics& g)
{

}

void BaseWindow::resized()
{
	Rectangle<int> parentArea(getBounds());

	editorArea.setBounds(parentArea.removeFromLeft(parentArea.getWidth() / 2));

	int height = parentArea.getHeight() / 2;
	propertiesArea.setBounds(parentArea.removeFromTop(height));
	toolsArea.setBounds(parentArea.removeFromTop(height));
	

}
