
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "BaseWindow.h"
#include "Tab.h"
#include "Tool.h"
#include "TabbedComponentWrapper.h"
#include "Tile.h"
#include <memory>
#include "json.hpp"
#include "PropertiesWindow.h"
#include <fstream>
using json = nlohmann::json;
class Editor : public BaseWindow, public ButtonListener, public MenuBarModel
{

public:
	////// MenuBarModel
	virtual StringArray getMenuBarNames()
	{
		return menus;
	}

	virtual PopupMenu getMenuForIndex(int topLevelMenuIndex,
		const String& menuName)
	{
		return *popups[topLevelMenuIndex];
	}

	virtual void menuItemSelected(int menuItemID, int topLevelMenuIndex)
	{
		if (topLevelMenuIndex == 0)
		{
			if (menuItemID == 1)
			{
				CreateModal create;
				if (openModal(create) == Modal::SUCCESS)
				{
					File file(File::getCurrentWorkingDirectory().getFullPathName() + "/" + create.getFileName() + ".json");
					mapContainer.addTab(create.getFileName(), true, new EditTab(create.getMapSize(), create.getTileSize(), 
						file, std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
					myLog(file.getFullPathName().toStdString());
				}	
			}
			else if (menuItemID == 2)
			{
				if (mapContainer.get().getNumTabs() > 0)
				{
					EditTab* tab =  static_cast<EditTab*>(static_cast<Viewport*>(mapContainer.get().getCurrentContentComponent())->getViewedComponent());
					
					if (!tab->isSaved())
					{
						FileChooser file("Save", File(), "*.json", false, false);
						if (file.browseForFileToSave(true))
						{
							File result = file.getResult();
							File finalFile;
							if (result.hasFileExtension(".json"))
							{
								finalFile = result;
							}
							else
							{
								finalFile = File(result.getFullPathName() + ".json");
							}
							const OwnedArray<Tile>& reftiles = tab->getNodes();
							json jsonWriter;

							/*
							My JSON format !
							    
							{
							    "tiles" :
								[
								   

								]

								"tileset" :
								[
								   {
								       "file",
									   "properties" : []
								   }
								]
							}

							*/
							
							for (int a = 0; a < reftiles.size(); a++)
							{
								jsonWriter["Tile"] += reftiles[a]->getSharedProperties()->getStrProperties().getProperties()[0].second.toStdString();
							}

							const OwnedArray<Tab>& reftilesettabs = tilesetContainer.getTabs();
							for (int a = 0; a < reftilesettabs.size(); a++)
							{
								std::map<std::string, std::string> tilePropertyMap;
								TilesetTab* const ptrTab = static_cast<TilesetTab*>(reftilesettabs[a]);
								const OwnedArray<Tile>& refnodes = ptrTab->getNodes();
								myLog("1");
								for (int b = 0; b < refnodes.size(); b++)
								{
									myLog("2");
									Properties& refproperties = refnodes[a]->getSharedProperties()->getStrProperties();

									if (refproperties.isEdited())
									{
										myLog("3");
										for (int c = refproperties.getDefaultPropertiesCount() - 1; c < refproperties.getCount(); c++)
										{
											myLog("4");
											tilePropertyMap[refproperties.getProperties()[c].first.toStdString()] = refproperties.getProperties()[c].second.toStdString();
										}
									}
									else myLog("not");
								}
								if (tilePropertyMap.size() > 0)
								{
									jsonWriter["tileset"][a]["file"] = ptrTab->getFile().getFileName().toStdString();
									jsonWriter["tileset"][a]["properties"] += tilePropertyMap;
								}
							}
							//for ( int a = 0; a < )
							/*
							for (int a = 0; a < reftiles.size(); a++)
							{
								Tile* ptrTile = reftiles[a];

							  	    const std::vector<std::pair<String, String>>& properties = ptrTile->getSharedProperties()->getStrProperties().getProperties();

									jsonWriter["Tile"] += properties[0].second.toStdString();
									for (int a = 1; a < properties.size(); a++)
									{
										jsonWriter["Tileset"][tab->getFile().getFileName().toStdString()]["Properties"][properties[0].second.toStdString()][properties[a].first.toStdString()] = properties[a].second.toStdString();
									}
							}
							*/
							std::ofstream fileWriter(finalFile.getFullPathName().toStdString());
							
							if (fileWriter.is_open())
							{
								fileWriter << std::setw(4) << jsonWriter;
							}
							else
							{
								WarningModal error("File wont open try again", Vector2i(300,300));
								openModal(error);
							}

						}
					}
				}
			}
		}
	}
private:
	StringArray menus;
	std::vector<std::unique_ptr<PopupMenu>> popups;
	////// end MenuBarModel
public:
	Editor(String name, Vector2i size)
		: BaseWindow(name, size), toolWindow("Editor tools", Vector2i(640, 320)),
		mapContainer(TabbedButtonBar::TabsAtTop, std::bind(&Editor::editorClose, this, std::placeholders::_1)),
		tilesetContainer(TabbedButtonBar::TabsAtTop, std::bind(&Editor::tilesetClose, this, std::placeholders::_1)),
		selectedTile(new Tile()), selectedTool(&default), editorMenuBarComponent(nullptr)
	{
		selectedTool->toggle();


		toolWindow.setContentNonOwned(&toolContentComponent, false);
		setContentNonOwned(&editorContentComponent, false);

		Colour tWhite((uint8)0, (uint8)0, (uint8)0, 0.5f);
		Colour tGrey((uint8)123, (uint8)123, (uint8)123, 0.5f);
		eraser.setImages(false, true, true, getImageFromFile("Asset/eraser.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		bucketfiller.setImages(false, true, true, getImageFromFile("Asset/bucketfiller.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		default.setImages(false, true, true, getImageFromFile("Asset/default.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		browseFile.setImages(false, true, true, getImageFromFile("Asset/fileopen.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);

		browseFile.setButtonText("...");
		eraser.setButtonText("er");
		bucketfiller.setButtonText("fil");
		default.setButtonText("def");

		browseFile.addListener(this);
		eraser.addListener(this);
		bucketfiller.addListener(this);
		default.addListener(this);

		addToToolContentComponent(eraser);
		addToToolContentComponent(bucketfiller);
		addToToolContentComponent(default);
		addToToolContentComponent(tilesetContainer.get());
		addToToolContentComponent(browseFile);

		addToEditorContentComponent(mapContainer.get());

	
		std::unique_ptr<PopupMenu> file(new PopupMenu());

		menus.add("File");
		
		file->addItem(1, "New", true, false);
		file->addItem(2, "Save", true, false);
		
		popups.push_back(std::move(file));

		editorMenuBarComponent.setModel(this);
		setMenuBarComponent(&editorMenuBarComponent);
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

	bool tilesetClose(Tab* tab)
	{

		TilesetTab* reftab = static_cast<TilesetTab*>(tab);
		OwnedArray<Tile>& reftilestileset = reftab->getNodes();
		bool hasPointer = false;
		OwnedArray<Tab>& edittabs = mapContainer.getTabs();
		myLog("tilesetClose()");
		for (int a = 0; a < edittabs.size(); a++)
		{
			EditTab* edittab = static_cast<EditTab*>(edittabs[a]);
			const OwnedArray<Tile>& reftileedit = edittab->getNodes();
			for (int i = 0; i < reftilestileset.size(); i++)
			{
				for (int j = 0; j < reftileedit.size(); j++)
				{
					if (reftileedit[j]->getSharedProperties() == reftilestileset[i]->getSharedProperties())
					{
						if (!hasPointer)
						{
							WarningModal warning("Are you sure?", Vector2i(300, 300));
							if (openModal(warning) == WarningModal::SUCCESS)
							{
								hasPointer = true;
							}
							else
							{
								return false;
							}
						}
						else
						{
							edittab->erase(reftileedit[j]);
						}
					}
				}
			}

		}

		return true;
	}

	bool editorClose(Tab* tab)
	{
		EditTab* reftab = static_cast<EditTab*>(tab);
		myLog("editorClose()");
		return true;
	}

	void resized() override
	{
		BaseWindow::resized();

		//mapContainer.setBounds(editorContentComponent.getLocalBounds().removeFromBottom(editorContentComponent.getLocalBounds().getHeight() - toolMenuBarComponent.getHeight()));
		mapContainer.setBounds(editorContentComponent.getLocalBounds());

		int width = 32;
		int height = 32;
		Rectangle<int> toolBounds(toolContentComponent.getLocalBounds());
		tilesetContainer.setBounds(toolBounds.removeFromTop(toolBounds.getHeight() - width));
		Vector2i pos(toolBounds.getX(), toolBounds.getY());
		default.setBounds(pos.x, pos.y, width, height);
		pos.x += width;
		eraser.setBounds(pos.x, pos.y, width, height);
		pos.x += width;
		bucketfiller.setBounds(pos.x, pos.y, width, height);
		pos.x = toolBounds.getWidth() / 2;
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
				
					Tab* tab = new TilesetTab(Vector2i(32, 32), file.getFullPathName(),
						std::bind(&Editor::setCurrentSelectedNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

					tilesetContainer.addTab(file.getFileNameWithoutExtension(), true, tab);
				}
			}
		}
		else
		{

			selectedTool->toggle();
			if (selectedTool != button)
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
			else
			{
				selectedNode->getSharedProperties()->getStrProperties().show();
			}
		}
	}

	void setCurrentSelectedNode(GridTabProto& currentTab, Tile* newNode, ModifierKeys mouseButton)
	{
		if (mouseButton == ModifierKeys::leftButtonModifier)
		{
			selectedTile->pointTo(newNode);/*
										   for (int a = 0; a < selectedTile->getSharedProperties()->getStrProperties().getProperties().size(); a++)
										   {
										   myLog(selectedTile->getSharedProperties()->getStrProperties().getProperties()[a].second.toStdString());
										   }*/
		}
		else if (mouseButton == ModifierKeys::rightButtonModifier)
		{
			newNode->getSharedProperties()->getStrProperties().show();
		}
	}

private:

	BaseWindow toolWindow;
	Component toolContentComponent;
	Component editorContentComponent;

	TabbedComponentWrapper mapContainer;
	TabbedComponentWrapper tilesetContainer;

	MenuBarComponent editorMenuBarComponent;

	ToolButton eraser;
	ToolButton bucketfiller;
	ToolButton default;
	ToolButton browseFile;

	ToolButton* selectedTool;
	Tile* selectedTile;





	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)
};
