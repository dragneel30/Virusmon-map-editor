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
#include "TabbedComponentWrapper.h"
#include "Tile.h"
//==============================================================================
/*
*/
class Editor : public BaseWindow, public ButtonListener
{
public:
	Editor(String name, Vector2i size)
		: BaseWindow(name, size), toolWindow("Editor tools", Vector2i(640, 320)),
		mapContainer(TabbedButtonBar::TabsAtTop), tilesetContainer(TabbedButtonBar::TabsAtTop), selectedTile(new Tile()), selectedTool(&default)
	{
		selectedTool->toggle();
		toolWindow.setContentNonOwned(&toolContentComponent, false);
		setContentNonOwned(&editorContentComponent, false);
		
		
		eraser.setImages(true, true, true, getImageFromFile("Asset/eraser.png"), 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
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

		mapContainer.addTab("Map 1", true, new EditTab(Vector2i(100, 100), Vector2i(32, 32), File(), std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		mapContainer.addTab("Map 2", true, new EditTab(Vector2i(100, 100), Vector2i(32, 32), File(), std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));


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

		int width = 32;
		int height = 32;
		Rectangle<int> toolBounds(toolContentComponent.getLocalBounds());
		tilesetContainer.setBounds(toolBounds.removeFromTop(toolBounds.getHeight()-width));
		Vector2i pos(toolBounds.getX(), toolBounds.getY());
		default.setBounds(pos.x, pos.y, width, height);
		pos.x += width;
		eraser.setBounds(pos.x, pos.y, width, height);
		pos.x += width;
		bucketfiller.setBounds(pos.x, pos.y, width, height);
		pos.x = toolBounds.getWidth()/2;
		browseFile.setBounds(pos.x, pos.y, width, height);
	}

	void updatePosition(Vector2i& pos, int width)
	{
		pos.x += 32;
		if (pos.x > width)
		{
			pos.y += 32;
			pos.x = 0;
		}
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
					File file = fileBrowser.getSelectedFile(0);
					
					Tab* tab = new TilesetTab(Vector2i(25, 6), Vector2i(32, 32), file, 
						std::bind(&Editor::setCurrentSelectedNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

					tilesetContainer.addTab(file.getFileNameWithoutExtension(), true, tab);
				}
			}
		}
		else
		{
			selectedTool->toggle();
			if ( selectedTool != button )
			{
				selectedTool = dynamic_cast<ToolButton*>(button);
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


	void fillNode(GridTabProto& currentTab, Tile* selectedNode, ModifierKeys mouseButton)
	{
		if (mouseButton != ModifierKeys::middleButtonModifier)
		{
			if (mouseButton == ModifierKeys::leftButtonModifier)
			{
				if (selectedTool != nullptr)
				{
					if (selectedTool == &default)
					{
						if (selectedTile->getSharedProperties() != nullptr)
						{
							selectedNode->pointTo(selectedTile);
						}
					}
					else if (selectedTool == &eraser)
					{
						EditTab& refCurrentTab = dynamic_cast<EditTab&>(currentTab);
						refCurrentTab.erase(selectedNode);
					}
					else if (selectedTool == &bucketfiller)
					{
						if (selectedTile != nullptr)
						{
							EditTab& refCurrentTab = dynamic_cast<EditTab&>(currentTab);
							refCurrentTab.fill(*selectedTile);
						}
					}
				}
			}
		}
	}

	void setCurrentSelectedNode(GridTabProto& currentTab, Tile* newNode, ModifierKeys mouseButton)
	{
		myLog("test");
		if (mouseButton == ModifierKeys::leftButtonModifier)
		{
			selectedTile->pointTo(newNode);
		}
	}
	/*
	OwnedArray<Tile> getClippedImagesFromFile(const String& filepath, Vector2i clipSize)
	{
		Image image = ImageFileFormat::loadFrom(File(filepath));
		int width = image.getWidth() / clipSize.x;
		int height = image.getHeight() / clipSize.y;
		OwnedArray<Tile> clippedImage;
		for (int lHeight = 0; lHeight < height; lHeight++)
		{
			for (int lWidth = 0; lWidth < width; lWidth++)
			{
				int size = (clippedImage.size() - 1) + 1;
				clippedImage.add(new Tile(image.getClippedImage(Rectangle<int>(lWidth * clipSize.x, lHeight * clipSize.y, clipSize.x, clipSize.y))));
			}
		}
		return clippedImage;
	}*/
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
	Tile* selectedTile;





    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Editor)
};
