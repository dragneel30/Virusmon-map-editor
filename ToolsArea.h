

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Util.h"
#include "TabbedComponentWrapper.h"
#include "Tab.h"


class ToolsArea : public Component, public ButtonListener
{
public:
	ToolsArea()
		: tilesetsHolder(TabbedButtonBar::Orientation::TabsAtTop)
	{
		addAndMakeVisible(browseFileButton);
		addAndMakeVisible(tilesetsHolder.get());
		browseFileButton.setButtonText("browse texture");
		
		browseFileButton.addListener(this);
	}

	~ToolsArea()
	{
		browseFileButton.removeListener(this);
	}

	void resized() override
	{
		Rectangle<int> thisBounds(getLocalBounds());
		browseFileButton.setBounds(thisBounds.removeFromBottom(5));
		tilesetsHolder.get().setBounds(thisBounds);
		
	}

	void buttonClicked(Button* activeButton) override
	{
		if (activeButton == &browseFileButton)
		{
			WildcardFileFilter fileFilter("*.jpg, *.png", "TEst", "Texture Image Files");

			FileBrowserComponent fileBrowser(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles, File("C:\\Users\\Lorence\\Desktop"), &fileFilter, nullptr);

			FileChooserDialogBox fileBrowserDialog("Selecte your texture file", "select jpeg or png file", fileBrowser, true, Colour(123, 123, 123));
			
			if (fileBrowserDialog.show())
			{
				if (fileBrowser.getNumSelectedFiles() > 0)
				{
					String filePath = fileBrowser.getSelectedFile(0).getFullPathName();
					OwnedArray<ImageComponent> clippedImages = getClippedImages(filePath, Vector2i(32, 32));
					tilesetsHolder.addTab("test", new GridTab(Vector2i(25,6), Vector2i(32, 32), std::move(clippedImages)));
				}
			}

		}
	}

private:
	Image selectedImage;

	TabbedComponentWrapper tilesetsHolder;
	TextButton browseFileButton;
















	OwnedArray<ImageComponent> getClippedImages(const String& filepath, Vector2i clipSize)
	{
		Image image = ImageFileFormat::loadFrom(File(filepath));
		int width = image.getWidth() / clipSize.x;
		int height = image.getHeight() / clipSize.y;
		//if (width * clipSize.x != image.getWidth()) width++;
		//if (height * clipSize.y != image.getHeight()) height++;
		//myLog(height);
		//myLog(width);
		OwnedArray<ImageComponent> clippedImage;

		for (int lHeight = 0; lHeight < height; lHeight++)
		{
			for (int lWidth = 0; lWidth < width; lWidth++)
			{
				int size = (clippedImage.size() - 1) + 1;
				clippedImage.add(new ImageComponent(std::to_string(size)));
				clippedImage[size]->setImage(image.getClippedImage(Rectangle<int>(lWidth * clipSize.x, lHeight * clipSize.y, clipSize.x, clipSize.y)));

			}
		}

		return clippedImage;
	}



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolsArea)
};
