

#pragma once


#include "../JuceLibraryCode//JuceHeader.h"
#include "Util.h"
#include "Vector2.h"
#include <vector>
#include <memory>
#include "Util.h"
#include <string>
#include "Tile.h"

class Tab : public Component
{
public:
	Tab(Vector2i tSize)
	{
		setSize(tSize.x, tSize.y);
		myLog("Tab");
	}

	virtual ~Tab() {}
protected:
};

class GridTabProto : public Tab
{
public:
	GridTabProto(Vector2i tSize, Vector2i nSize, const File& imageFile, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: Tab(tSize), nodeSize(nSize), nodeClickCallback(pCallback), file(imageFile), mListener(this)
	{
		addMouseListener(&mListener, true);
		myLog("GridTabProto");
	}
	virtual ~GridTabProto()
	{
	}
	bool contains(const Tile* tile)
	{
		return nodes.contains(tile);
	}
	void resized() override
	{
		int x = 0, y = 0;
		for (int a = 0; a < nodes.size(); a++)
		{
			nodes[a]->setBounds(x, y, nodeSize.x, nodeSize.y);
			x += nodeSize.x;
			if (x >= getWidth())
			{
				x = 0;
				y += nodeSize.y;
			}
		}
		
	}

	const File& getFile()
	{
		return file;
	}
	OwnedArray<Tile>& getNodes() { return nodes; }
protected:
	File file;
	std::function<void(GridTabProto&, Tile*, ModifierKeys)> nodeClickCallback;
	OwnedArray<Tile> nodes;
	Vector2i nodeSize;
	virtual void generateGrid() = 0;
	class GridTabProtoListener : public MouseListener
	{
	public:
		GridTabProtoListener(GridTabProto* pBroadcaster)
			: broadcaster(pBroadcaster)
		{
		}
		virtual ~GridTabProtoListener()
		{
			broadcaster = nullptr;
		}
		virtual void mouseDown(const MouseEvent& event) override 
		{
			Vector2i pos = event.getEventRelativeTo(broadcaster).getPosition();
			nodeClicked(pos, event);
		}
		void nodeClicked(Vector2i mousePos, const MouseEvent& event)
		{
			for (int a = 0; a < broadcaster->nodes.size(); a++)
			{
				Rectangle<int> bounds = broadcaster->nodes[a]->getBounds();
				//myLog(std::to_string(bounds.getX()) + " " + std::to_string(bounds.getY()));
				if (mousePos.x >= bounds.getX() && mousePos.y >= bounds.getY() && mousePos.x <= bounds.getX() + bounds.getWidth() && mousePos.y <= bounds.getY() + bounds.getHeight())
				{
					if (event.mods.isRightButtonDown())
					{
						broadcaster->nodeClickCallback(*broadcaster, broadcaster->nodes[a], event.mods.rightButtonModifier);
					}
					else if (event.mods.isLeftButtonDown())
					{
						broadcaster->nodeClickCallback(*broadcaster, broadcaster->nodes[a], event.mods.leftButtonModifier);
					}
					return;
				}
			}
		}

		virtual void mouseDrag(const MouseEvent& event) override
		{
			Vector2i pos = event.getEventRelativeTo(broadcaster).getPosition();
			nodeClicked(pos, event);
		}
	private:
		GridTabProto* broadcaster;
	};
	GridTabProtoListener mListener;
};


class EditTab : public GridTabProto
{
public:
	bool isSaved() { return saved; }
	bool setSave(bool save) { saved = save; }
	EditTab(Vector2i tSize, Vector2i nSize, const File& file, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: GridTabProto(Vector2i(tSize.x * nSize.x, tSize.y * nSize.y), nSize, file, pCallback), saved(false)
	{
		generateGrid();
	}
	virtual ~EditTab()
	{
		removeMouseListener(&mListener);
	}
	virtual void generateGrid() override
	{
		int size = (getWidth() / nodeSize.x) * (getHeight() / nodeSize.y);
		for (int i = 0; i < size; i++)
		{
			nodes.add(new Tile(getDefaultNode().getSharedProperties()));
			addAndMakeVisible(nodes[i]);
		}
		resized();
	}
	void fill(Tile& tile)
	{
		for (int a = 0; a < nodes.size(); a++)
		{
			nodes[a]->pointTo(&tile);
		}
	}

	void erase(Tile* tile)
	{
		tile->pointTo(&getDefaultNode());
	}
private:
	static Tile& getDefaultNode()
	{
		static Tile defaultTile(std::make_shared<TileType>(intUID(-1), getImageFromFile("Asset/defaultNodeImage.png")));
		return defaultTile;
	}
	bool saved;
};

class TilesetTab : public GridTabProto
{
public:
	
	TilesetTab(Vector2i nSize, const File& pfile, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: GridTabProto(loadAndCalculateGridSize(pfile, nSize), nSize, pfile, pCallback)
	{

		generateGrid();
	}	

	virtual ~TilesetTab()
	{
	}
	Vector2i loadAndCalculateGridSize(const File& pfile, Vector2i nSize)
	{
		Image image = ImageFileFormat::loadFrom(pfile);
	    // had to do this because if i make it a member then ill be using uninitialized pointer (ie its internal pointer)
		Vector2i gridSize(image.getWidth() - (image.getWidth()/ nSize.x), image.getHeight() - (image.getHeight()/ nSize.y));
		myLog("gridsize");
		myLog(gridSize.x);
		myLog(gridSize.y);
		return gridSize;
	}
	virtual void generateGrid() override
	{
		Image image = ImageFileFormat::loadFrom(file);
		// had to do this because when i make it a member then ill be using uninitialized pointer (ie its internal pointer)

		int width = getWidth() / nodeSize.x;
		int height = getHeight() / nodeSize.y;
		//myLog(width);
		//myLog(height);
		intUID id = 1;
		int lineHeight = 0;
		for (int lHeight = 0; lHeight < height; lHeight++)
		{
			int lineWidth = 0;
			for (int lWidth = 0; lWidth < width; lWidth++)
			{
				Rectangle<int> bounds((lWidth * 32) + lineWidth++, (lHeight * 32) + lineHeight, 32, 32);
				nodes.add(new Tile(std::make_shared<TileType>(id++, image.getClippedImage(Rectangle<int>(bounds)))));
				addAndMakeVisible(nodes[nodes.size() - 1]);
			}
			lineHeight++;
		}
		resized();
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TilesetTab)
};
