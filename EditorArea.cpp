
#include "../JuceLibraryCode/JuceHeader.h"
#include "EditorArea.h"
#include <string>
#include "Vector2.h"
#include "Tab.h"

EditorArea::EditorArea()
	: tab(TabbedButtonBar::Orientation::TabsAtTop)
{
	addAndMakeVisible(tab.get());/*
	std::vector<ImageComponent> image(10000);
	Image image = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("Asset/test.jpg"));
	for (int a = 0; a < ; a++)
	{
		image[a].setImage(image);
	}*/
	tab.addTab("Test 1", new GridTab(Vector2i(100, 100), Vector2i(32, 32)));
	tab.addTab("Test 2", new GridTab(Vector2i(100, 100), Vector2i(32, 32)));
}

EditorArea::~EditorArea()
{
}

void EditorArea::paint (Graphics& g)
{
}

void EditorArea::resized()
{
	Rectangle<int> area(getLocalBounds());
	tab.setBounds(area);
}
