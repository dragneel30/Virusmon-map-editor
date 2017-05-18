/*
  ==============================================================================

    Editor.h
    Created: 17 May 2017 7:59:12am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BaseWindow.h"
#include "Tab.h"
#include "Tool.h"
//==============================================================================
/*
*/
class Editor : public BaseWindow, public ButtonListener
{
public:
	Editor(String name, Vector2i size)
		: BaseWindow(name, size), toolWindow("Editor tools",Vector2i(800,800)),
		mapContainer(TabbedButtonBar::TabsAtTop), tilesetContainer(TabbedButtonBar::TabsAtTop), selectedTile(nullptr), selectedTool(&default)
	{
		selectedTool->toggle();
		size.x = 768;
		size.y = 768;
		toolWindow.setContentNonOwned(&toolContentComponent, false);
		setContentNonOwned(&editorContentComponent, false);
		
		
		eraser.setImages(false, true, true, getImageFromFile("Asset/eraser.png"), 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
		bucketfiller.setImages(false, true, true, getImageFromFile("Asset/bucketfiller.png"), 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
		default.setImages(false, true, true, getImageFromFile("Asset/default.png"), 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
		browseFile.setImages(false, true, true, getImageFromFile("Asset/fileopen.png"), 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
	
		browseFile.setButtonText("...");
		browseFile.addListener(this);
		eraser.setButtonText("er");
		eraser.addListener(this);
		bucketfiller.setButtonText("fil");
		bucketfiller.addListener(this);
		default.setButtonText("def");
		default.addListener(this);
		
		addToToolContentComponent(eraser);
		addToToolContentComponent(bucketfiller);
		addToToolContentComponent(default);
		addToToolContentComponent(tilesetContainer.get());
		addToToolContentComponent(browseFile);
		

		addToEditorContentComponent(mapContainer.get());

		mapContainer.addTab("Test 1", true, new GridTab(Vector2i(100, 100), Vector2i(32, 32), std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		mapContainer.addTab("Test 2", true, new GridTab(Vector2i(100, 100), Vector2i(32, 32), std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


	}
	~Editor() 
	{
		selectedTool = nullptr;
		selectedTile = nullptr;
	}

	void closeButtonPressed() override
	{
		JUCEApplication::getInstance()->systemRequestedQuit();
	}


	void resized() override
	{
		BaseWindow::resized();
		mapContainer.setBounds(editorContentComponent.getLocalBounds());

		Rectangle<int> toolBounds(toolContentComponent.getLocalBounds());
		int width = 32;
		int height = 32;
		Vector2i pos(toolBounds.getX(), toolBounds.getY());
		default.setBounds(pos.x, pos.y, width, height);
		updatePosition(pos, toolBounds.getWidth());
		eraser.setBounds(pos.x, pos.y, width, height);
		updatePosition(pos, toolBounds.getWidth());
		bucketfiller.setBounds(pos.x, pos.y, width, height);
		updatePosition(pos, toolBounds.getWidth());
		browseFile.setBounds(pos.x, pos.y, width, height);
		tilesetContainer.setBounds(toolBounds.removeFromBottom(500));
	}

	void updatePosition(Vector2i& pos, int width)
	{
		if (pos.x >= width)
		{
			pos.y += 32;
			pos.x = 0;
		}
		else
			pos.x += 32;
	}
	virtual void buttonClicked(Button* button) override
	{
		if (button == &browseFile)
		{
			WildcardFileFilter fileFilter("*.jpg, *.png", "TEst", "Texture Image Files");

			FileBrowserComponent fileBrowser(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles, File("C:\\Users\\Lorence\\Desktop"), &fileFilter, nullptr);
			FileChooserDialogBox fileBrowserDialog("Selecte your texture file", "select jpeg or png file", fileBrowser, true, Colour(123, 123, 123));

			if (fileBrowserDialog.show())
			{
				if (fileBrowser.getNumSelectedFiles() > 0)
				{
					String filePath = fileBrowser.getSelectedFile(0).getFullPathName();
					OwnedArray<ImageComponent> clippedImages = getClippedImagesFromFile(filePath, Vector2i(32, 32));
					GridTab* tab = new GridTab(Vector2i(25, 6), Vector2i(32, 32), std::bind(&Editor::setCurrentSelectedNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), std::move(clippedImages));

					tilesetContainer.addTab("test", true, tab);
				}
			}
		}
		else
		{
			selectedTool->toggle();
			if ( selectedTool != button )
			{
				selectedTool = static_cast<ToolButton*>(button);
			}
			else
			{
				selectedTool = &default;
			}
			selectedTool->toggle();
		}
	}

	void addToToolContentComponent(Component& component)
	{
		toolContentComponent.addAndMakeVisible(component);
	}

	void addToEditorContentComponent(Component& component)
	{
		editorContentComponent.addAndMakeVisible(mapContainer.get());
	}

	void fillNode(GridTab& currentTab, ImageComponent& selectedImageComponent, ModifierKeys mouseButton)
	{
		if (mouseButton != ModifierKeys::middleButtonModifier)
		{
			if (mouseButton == ModifierKeys::leftButtonModifier)
			{
				if (selectedTool != nullptr)
				{
					if (selectedTool == &default)
					{
						if (selectedTile != nullptr)
						{
							selectedImageComponent.setImage(*selectedTile);
						}
					}
					else if (selectedTool == &eraser)
					{
						selectedImageComponent.setImage(GridTab::getDefaultNodeImage());
					}
					else if (selectedTool == &bucketfiller)
					{
						if (selectedTile != nullptr)
						{
							currentTab.fill(*selectedTile);
						}
					}
				}
			}
		}
	}

	void setCurrentSelectedNode(GridTab& currentTab, ImageComponent& newNode, ModifierKeys mouseButton)
	{
		if (mouseButton == ModifierKeys::leftButtonModifier)
			selectedTile = &newNode.getImage();
	}

	OwnedArray<ImageComponent> getClippedImagesFromFile(const String& filepath, Vector2i clipSize)
	{
		Image image = ImageFileFormat::loadFrom(File(filepath));
		int width = image.getWidth() / clipSize.x;
		int height = image.getHeight() / clipSize.y;
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
private:

	BaseWindow toolWindow;
	Component toolContentComponent;
	Component editorContentComponent;

	TabbedComponentWrapper mapContainer;
	TabbedComponentWrapper tilesetContainer;


	ToolButton eraser;
	ToolButton bucketfiller;
	ToolButton default;
	ToolButton browseFile;

	ToolButton* selectedTool;
	Image const* selectedTile;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
