
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
class MenuModel : public MenuBarModel
{
public:
	MenuModel()
		: MenuBarModel()
	{

	}
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
	}
private:
	StringArray menus;
	std::vector<std::unique_ptr<PopupMenu>> popups;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuModel)
};
class ToolWindow : public BaseWindow
{
public:
	ToolWindow(const String& name, Vector2i size)
		: BaseWindow(name, size) {}
	void closeButtonPressed() override
	{
		setVisible(false);
	}
};
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
				CreateEditTab create;
				if (openModal(create) == Modal::SUCCESS)
				{ 
					File file(File::getCurrentWorkingDirectory().getFullPathName() + "/" + create.getFileName() + ".json");
					mapContainer.addTab(create.getFileName(), true, new EditTab(Vector2i(create.getColumnCount(), create.getRowCount()), Vector2i(create.getTileWidth(), create.getTileHeight()), 
						file, std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), create.getOrientation(), create.getRenderOrder()));
				}	
			}
			else if (menuItemID == 2)
			{
				if (mapContainer.get().getNumTabs() > 0)
				{
					EditTab* tab =  static_cast<EditTab*>(static_cast<Viewport*>(mapContainer.get().getCurrentContentComponent())->getViewedComponent());
					
					if (!tab->isSaved())
					{
						FileChooser file("Save", File(), "*.json");
						if (file.browseForFileToSave(true))
						{
							File result = file.getResult();
							File finalFile;
							if (!result.hasFileExtension(".json"))
							{
								result = File(result.getFullPathName() + ".json");
							}
							const OwnedArray<Tile>& reftiles = tab->getNodes();
							json jsonWriter;

							jsonWriter["width"] = std::to_string(tab->getWidth());
							jsonWriter["height"] = std::to_string(tab->getHeight());
							jsonWriter["column"] = std::to_string(tab->getSizeBasedNodeCount().x);
							jsonWriter["row"] = std::to_string(tab->getSizeBasedNodeCount().y);
							jsonWriter["tile_width"] = std::to_string(tab->getNodeSize().x);
							jsonWriter["tile_height"] = std::to_string(tab->getNodeSize().y);
							jsonWriter["orientation"] = tab->getOrientation().toStdString();
							jsonWriter["renderorder"] = tab->getRenderOrder().toStdString();
							for (int a = 0; a < reftiles.size(); a++)
							{
								jsonWriter["tiles"] += reftiles[a]->getSharedProperties()->getStrProperties().getProperties()[0].value.toStdString();
							}

							const OwnedArray<Tab>& reftilesettabs = tilesetContainer.getTabs();
							myLog(reftilesettabs.size());
							for (int a = 0; a < reftilesettabs.size(); a++)
							{
								TilesetTab* const ptrTab = static_cast<TilesetTab*>(reftilesettabs[a]);
								const OwnedArray<Tile>& refnodes = ptrTab->getNodes();
								for (int b = 0; b < refnodes.size(); b++)
								{
									std::map<std::string, std::string> tilePropertyMap;
									Properties& refproperties = refnodes[b]->getSharedProperties()->getStrProperties();

									if (refproperties.isEdited())
									{
										for (int c = refproperties.getDefaultPropertiesCount(); c < refproperties.getCount(); c++)
										{
											tilePropertyMap[refproperties.getProperties()[c].key.toStdString()] = refproperties.getProperties()[c].value.toStdString();
										}
									}
									if (tilePropertyMap.size() > 0)
									{
										jsonWriter["tileset"][a]["properties"] += tilePropertyMap;
										jsonWriter["tileset"][a]["file"] = ptrTab->getFile().getFileName().toStdString();
										jsonWriter["tileset"][a]["name"] = ptrTab->getName().toStdString();
										jsonWriter["tileset"][a]["image_width"] = std::to_string(ptrTab->getImageSize().x);
										jsonWriter["tileset"][a]["image_height"] = std::to_string(ptrTab->getImageSize().y);
										jsonWriter["tileset"][a]["tile_width"] = std::to_string(ptrTab->getNodeSize().x);
										jsonWriter["tileset"][a]["tile_height"] = std::to_string(ptrTab->getNodeSize().y);
										jsonWriter["tileset"][a]["tile_count"] = std::to_string(ptrTab->getTileCount());
										jsonWriter["tileset"][a]["column"] = std::to_string(ptrTab->getSizeBasedNodeCount().x);
										jsonWriter["tileset"][a]["row"] = std::to_string(ptrTab->getSizeBasedNodeCount().y);
										jsonWriter["tileset"][a]["margin"] = std::to_string(ptrTab->getMargin());
										jsonWriter["tileset"][a]["spacing"] = std::to_string(ptrTab->getSpacing());
									}
								}
							}
							std::ofstream fileWriter(result.getFullPathName().toStdString());
							
							if (fileWriter.is_open())
							{
								fileWriter << std::setw(4) << jsonWriter;
								tab->setSave(true);
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
			else if (menuItemID == 3)
			{
				FileChooser file("Select file", File(), "*.json", false);
				if (file.browseForFileToOpen())
				{
					myLog("1");
					File result = file.getResult();
					OwnedArray<Tab>& refmapstabs = mapContainer.getTabs();
					for (int a = 0; a < refmapstabs.size(); a++)
					{
						EditTab* refedittab = static_cast<EditTab*>(refmapstabs[a]);
						if (refedittab->getFile() == result)
						{
							return;
						}
					}
					if (result.hasFileExtension(".json"))
					{
						myLog("2");
						std::ifstream filereader(result.getFullPathName().toStdString());
						if (filereader.is_open())
						{
							myLog("3");
							json jsonloader;
							filereader >> jsonloader;
							if (jsonloader.find("tileset") != jsonloader.end())
							{
								for (int i = 0; i < jsonloader["tileset"].size(); i++)
								{
									myLog("test");
									Tab* tab;
									Vector2i sizeInTile(std::stoi(jsonloader["tileset"][i]["column"].get<std::string>()),
									std::stoi(jsonloader["tileset"][i]["row"].get<std::string>()));

									Vector2i imageSize(std::stoi(jsonloader["tileset"][i]["image_height"].get<std::string>()),
									std::stoi(jsonloader["tileset"][i]["image_width"].get<std::string>()));

									int tile_count = std::stoi(jsonloader["tileset"][i]["tile_count"].get<std::string>());

									Vector2i tileSize(std::stoi(jsonloader["tileset"][i]["tile_height"].get<std::string>()),
										std::stoi(jsonloader["tileset"][i]["tile_width"].get<std::string>()));

									int spacing = std::stoi(jsonloader["tileset"][i]["spacing"].get<std::string>());
									int margin = std::stoi(jsonloader["tileset"][i]["margin"].get<std::string>());

									Vector2i tilesetSize(sizeInTile.x * tileSize.x, sizeInTile.y * tileSize.y);
									String name = jsonloader["tileset"][i]["name"].get<std::string>();
									String strfile = jsonloader["tileset"][i]["file"].get<std::string>();
									File file("C:/users/lorence/desktop/" + strfile);

									
									tab = new TilesetTab(tilesetSize, sizeInTile, tileSize, imageSize, margin, spacing, file, name, 
										std::bind(&Editor::setCurrentSelectedNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
									
									tilesetContainer.addTab(name, true, tab);
									
									
								}
							}


							Vector2i mapSize(std::stoi(jsonloader["width"].get<std::string>()), std::stoi(jsonloader["height"].get<std::string>()));
							Vector2i tileSize(std::stoi(jsonloader["tile_width"].get<std::string>()), std::stoi(jsonloader["tile_height"].get<std::string>()));
							Vector2i sizeInTile(std::stoi(jsonloader["row"].get<std::string>()), std::stoi(jsonloader["column"].get<std::string>()));
							std::string orientation = jsonloader["orientation"];
							std::string renderorder = jsonloader["renderorder"];
							Tab* tab = new EditTab(mapSize, tileSize, result, std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), 
								orientation, renderorder, sizeInTile, true);
							
							mapContainer.addTab(result.getFileNameWithoutExtension(), true, tab);


							EditTab* reftab = static_cast<EditTab*>(mapContainer.getTab(mapContainer.get().getCurrentTabIndex()));


							std::vector<std::string> tiles = jsonloader["tiles"];


							OwnedArray<Tab>& reftilesettabs = tilesetContainer.getTabs();
							for (int a = 0; a < tiles.size(); a++)
							{
								if (tiles[a] == "-1")
								{
									reftab->addNode(&EditTab::getDefaultNode());
								}
								else
								{
									for (int b = 0; b < reftilesettabs.size(); b++)
									{

										TilesetTab* reftilesettab = static_cast<TilesetTab*>(reftilesettabs[b]);
										if (std::stoi(tiles[a]) <= reftilesettab->getTileCount())
										{
											reftab->addNode(reftilesettab->getNodes()[std::stoi(tiles[a]) - 1]);
											break;
										}
									}
								}
							}
							reftab->resized();


							
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
		selectedTile(new Tile()), selectedTool(&default), editorMenuBarComponent(new MenuBarComponent(nullptr))
	{
		selectedTool->toggle();

		toolWindow.setContentOwned(&toolContentComponent, false);
		setContentOwned(&editorContentComponent, false);

		Colour tWhite((uint8)0, (uint8)0, (uint8)0, 0.5f);
		Colour tGrey((uint8)123, (uint8)123, (uint8)123, 0.5f);
		eraser.setImages(false, true, true, getImageFromFile("Asset/eraser.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		bucketfiller.setImages(false, true, true, getImageFromFile("Asset/bucketfiller.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		default.setImages(false, true, true, getImageFromFile("Asset/default.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		browseFile.setImages(false, true, true, getImageFromFile("Asset/fileopen.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);
		newTileset.setImages(false, true, true, getImageFromFile("Asset/new.png"), 1.0f, Colour(), Image(), 1.0f, tWhite, Image(), 1.0f, tGrey);

		browseFile.addListener(this);
		eraser.addListener(this);
		bucketfiller.addListener(this);
		default.addListener(this);
		newTileset.addListener(this);

		addToToolContentComponent(newTileset);
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
		file->addItem(3, "Open", true, false);
		
		popups.push_back(std::move(file));

		editorMenuBarComponent->setModel(this);
		setMenuBarComponent(editorMenuBarComponent);

		

		mapContainer.addTab("Virusmon map", true, new EditTab(Vector2i(12,12), Vector2i(32,32), File(), std::bind(&Editor::fillNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),"orthogonal","right-bottom"));
	}
	~Editor()
	{
		setMenuBarComponent(nullptr);
		browseFile.removeListener(this);
		eraser.removeListener(this);
		bucketfiller.removeListener(this);
		default.removeListener(this);
		newTileset.removeListener(this);
		delete selectedTile;
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
		newTileset.setBounds(pos.x, pos.y, width, height);
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
		if (button == &newTileset)
		{
			CreateTilesetTab create;
			if (openModal(create) == Modal::SUCCESS)
			{
				Tab* tab = new TilesetTab(Vector2i(create.getTileWidth(), create.getTileHeight()), create.getFile(), create.getFileName(),
					std::bind(&Editor::setCurrentSelectedNode, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3), create.getMargin(), create.getSpacing());
				tilesetContainer.addTab(create.getFile().getFileNameWithoutExtension(), true, tab);
			}
		}
		
		else
		{

			selectedTool->toggle();
			if (selectedTool != button)
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
						EditTab& refCurrentTab = static_cast<EditTab&>(currentTab);
						refCurrentTab.erase(selectedNode);
					}
					else if (selectedTool == &bucketfiller)
					{
						if (selectedTile != nullptr)
						{
							EditTab& refCurrentTab = static_cast<EditTab&>(currentTab);
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
			selectedTile->pointTo(newNode);
		}
		else if (mouseButton == ModifierKeys::rightButtonModifier)
		{
			newNode->getSharedProperties()->getStrProperties().show();
		}
	}

private:

	MenuBarComponent* editorMenuBarComponent;
	ToolWindow toolWindow;
	Component toolContentComponent;
	Component editorContentComponent;

	TabbedComponentWrapper mapContainer;
	TabbedComponentWrapper tilesetContainer;


	ToolButton eraser;
	ToolButton bucketfiller;
	ToolButton default;
	ToolButton browseFile;
	ToolButton newTileset;

	ToolButton* selectedTool;
	Tile* selectedTile;





	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Editor)
};
