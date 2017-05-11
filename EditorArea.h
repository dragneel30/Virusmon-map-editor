
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TabbedComponentWrapper.h"
#include "ToolsArea.h"
class EditorArea : public Component
{
public:
    EditorArea();
    ~EditorArea();

    void paint (Graphics&) override;
    void resized() override;

	
private:
	void fillNode(const ImageComponent& selectedImageComponent)
	{
		myLog("fillnode");
		if (ToolsArea::getCurrentSelectedNode() != nullptr)
		{
			ImageComponent& tempref = const_cast<ImageComponent&>(selectedImageComponent);
			tempref.setImage(*ToolsArea::getCurrentSelectedNode());
		}
	}
	TabbedComponentWrapper tab;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorArea)
};
