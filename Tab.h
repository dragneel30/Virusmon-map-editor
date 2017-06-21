

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
	void addNode(Tile* newNode)
	{
		Tile* node = new Tile();
		addAndMakeVisible(node);
		node->pointTo(newNode);
		nodes.add(node);
	}
	GridTabProto(Vector2i tSize, Vector2i sBaseNodeCount, Vector2i nSize, const File& imageFile, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: Tab(tSize), nodeSize(nSize), sizeBasedNodeCount(sBaseNodeCount), nodeClickCallback(pCallback), file(imageFile), mListener(this)
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
		myLog("Size: " + std::to_string(nodes.size()));
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
    OwnedArray<Tile>& getNodes()  { return nodes; }
	Vector2i getNodeSize() { return nodeSize; }
	Vector2i getSizeBasedNodeCount() {
		return sizeBasedNodeCount;
	}
protected:
	File file;
	std::function<void(GridTabProto&, Tile*, ModifierKeys)> nodeClickCallback;
	OwnedArray<Tile> nodes;
	Vector2i nodeSize;
	Vector2i sizeBasedNodeCount; 
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
	String orientation;
	String renderOrder;
public:
	bool isSaved() { return saved; }
	void setSave(bool save) { saved = save; }
	EditTab(Vector2i sizeInTile, Vector2i nSize, const File& file, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback, String pOrientation, String pRenderOrder)
		:	EditTab(Vector2i(sizeInTile.x * nSize.x, sizeInTile.y * nSize.y), nSize, file, pCallback, pOrientation, pRenderOrder, sizeInTile, false)
	{
		generateGrid();
	}

	EditTab(Vector2i tSize, Vector2i nSize, const File& file, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback, String pOrientation, String pRenderOrder, Vector2i sizeInTile, bool pSave)
		: GridTabProto(tSize, sizeInTile, nSize, file, pCallback), orientation(pOrientation), renderOrder(pRenderOrder), saved(pSave)
	{

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
	String getOrientation() { return orientation; }
	String getRenderOrder() { return renderOrder; }
	static Tile& getDefaultNode()
	{
		static Tile defaultTile(std::make_shared<TileType>(intUID(-1), getImageFromFile("Asset/defaultNodeImage.png")));
		return defaultTile;
	}
private:
	bool saved;
};

class TilesetTab : public GridTabProto
{
	Vector2i imageSize;
	int margin;
	int spacing;
	String name;
public:
	Vector2i getImageSize() { return imageSize; }
	int getMargin() { return margin; }
	int getSpacing() { return spacing; }
	int getTileCount() { return nodes.size(); }
	String getName() { return name; }
	TilesetTab(Vector2i nSize, const File& pfile, const String& name, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback, int pMargin, int pSpacing)
		: TilesetTab(calculateGridSize(pfile, pMargin, pSpacing, nSize), Vector2i(), nSize, Vector2i(), pMargin, pSpacing, pfile, name, pCallback)
	{

		sizeBasedNodeCount = Vector2i(getWidth() / nSize.x, getHeight() / nSize.y);

	}

	TilesetTab(Vector2i tSize, Vector2i sizeInTile, Vector2i nSize, Vector2i pImageSize, int pMargin, int pSpacing, const File& pFile, const String& pName, const std::function<void(GridTabProto&, Tile*, ModifierKeys)>& pCallback)
		: GridTabProto(tSize, sizeInTile, nSize, pFile, pCallback), imageSize(pImageSize), margin(pMargin), spacing(pSpacing), name(pName)
	{
		generateGrid();
	}
	virtual ~TilesetTab()
	{
	}
	Vector2i calculateGridSize(const File& pfile, int margin, int spacing, Vector2i nodeSize)
	{
		Image image = ImageFileFormat::loadFrom(pfile);
	    // had to do this because if i make it a member then ill be using uninitialized pointer when calculateGridSize() used it (ie its internal pointer)
		int offset = margin + spacing;
	    Vector2i size(image.getWidth() - offset, image.getHeight() - offset);
		size.x -= (size.x % nodeSize.x);
		size.y -= (size.y % nodeSize.y);
		return size;
		
	}
	virtual void generateGrid() override
	{
		Image image = ImageFileFormat::loadFrom(file);
		// had to do this because when i make it a member then ill be using uninitialized pointer when calculateGridSize() used it (ie its internal pointer)
		int width = getWidth() / nodeSize.x;
		int height = getHeight() / nodeSize.y;
		imageSize = Vector2i(image.getWidth(), image.getHeight());
		intUID id = 1;
		int spaceHeight = margin;
		for (int lHeight = 0; lHeight < height; lHeight++)
		{
			int spaceWidth = margin;
			for (int lWidth = 0; lWidth < width; lWidth++)
			{
				Rectangle<int> bounds((lWidth * nodeSize.x) + spaceWidth, (lHeight * nodeSize.y) + spaceHeight, nodeSize.x, nodeSize.y);
				nodes.add(new Tile(std::make_shared<TileType>(id++, image.getClippedImage(Rectangle<int>(bounds)))));
				addAndMakeVisible(nodes[nodes.size() - 1]);
				spaceWidth += spacing;
			}
			spaceHeight += spacing;
		}
		resized();
	}
	intUID getLastID()
	{
		return nodes[nodes.size() - 1]->getSharedProperties()->getID();
	}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TilesetTab)
};
