

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
		myLog(tSize.x);
		myLog(tSize.y);
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

	OwnedArray<Tile>& getNodes() { return nodes; }
protected:
	std::function<void(GridTabProto&, Tile*, ModifierKeys)> nodeClickCallback;
	OwnedArray<Tile> nodes;
	Vector2i nodeSize;
	File file;
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
			Vector2i pos = event.getEventRelativeTo(broadcaster->getParentComponent()).getPosition();
			nodeClicked(pos, event);
		}
		void nodeClicked(Vector2i mousePos, const MouseEvent& event)
		{
			for (int a = 0; a < broadcaster->nodes.size(); a++)
			{
				Rectangle<int> bounds = broadcaster->nodes[a]->getBounds();
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
			Vector2i pos = event.getEventRelativeTo(broadcaster->getParentComponent()).getPosition();
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
	EditTab(Vector2i tSize, Vector2i nSize, const File& file, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: GridTabProto(Vector2i(tSize.x * nSize.x, tSize.y * nSize.y), nSize, file, pCallback)
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
		static Tile defaultTile(new TileType(intUID(-1), getImageFromFile("Asset/defaultNodeImage.png")));
		return defaultTile;
	}
};

class TilesetTab : public GridTabProto
{
public:
	
	TilesetTab(Vector2i tSize, Vector2i nSize, const File& file, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: GridTabProto(Vector2i(tSize.x * nSize.x, tSize.y * nSize.y), nSize, file, pCallback)
	{

		generateGrid();
	}	

	virtual ~TilesetTab()
	{
	}

	virtual void generateGrid() override
	{
		Image image = ImageFileFormat::loadFrom(file);
		int width = image.getWidth() / nodeSize.x;
		int height = image.getHeight() / nodeSize.y;
		intUID id = 0;
		for (int lHeight = 0; lHeight < height; lHeight++)
		{
			for (int lWidth = 0; lWidth < width; lWidth++)
			{
				nodes.add(new Tile(new TileType(id++, image.getClippedImage(Rectangle<int>(lWidth * nodeSize.x, lHeight * nodeSize.y, nodeSize.x, nodeSize.y)))));
				addAndMakeVisible(nodes[nodes.size() - 1]);
			}
		}
		resized();
	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TilesetTab)
};
