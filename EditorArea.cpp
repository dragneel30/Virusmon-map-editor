
#include "../JuceLibraryCode/JuceHeader.h"
#include "EditorArea.h"
#include <string>
#include "Vector2.h"
#include "Tab.h"

EditorArea::EditorArea()
	: tab(TabbedButtonBar::Orientation::TabsAtTop)
{
	addAndMakeVisible(tab.get());

	tab.addTab("Test 1", new GridTab(Vector2i(100, 100), Vector2i(32, 32), std::bind(&EditorArea::fillNode, this, std::placeholders::_1)));
	tab.addTab("Test 2", new GridTab(Vector2i(100, 100), Vector2i(32, 32), std::bind(&EditorArea::fillNode, this, std::placeholders::_1)));
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
