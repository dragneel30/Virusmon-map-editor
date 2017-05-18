

#pragma once


#include "../JuceLibraryCode//JuceHeader.h"
#include "Util.h"
#include "Vector2.h"
#include <vector>
#include <memory>
#include "Util.h"
#include <string>
class Tab : public Component
{
public:
	Tab(Vector2i tSize)
		: tabSize(tSize)
	{

	}

	virtual ~Tab() {}
protected:
	Vector2i tabSize;
};

class GridTab;

class GridTab : public Tab 
{

	class GridTabListener : public MouseListener, public KeyListener
	{
	public:
		GridTabListener(GridTab* pBroadcaster)
			: broadcaster(pBroadcaster)
		{

		}
		bool keyPressed(const KeyPress& key, Component* originatingComponent) override { return false;  }
		virtual ~GridTabListener()
		{
			broadcaster = nullptr;
		}
		virtual void mouseDown(const MouseEvent& event)
		{
			initialMouseDownPos = event.getEventRelativeTo(broadcaster->getParentComponent()).getPosition();
			nodeClicked(initialMouseDownPos, event);
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
						broadcaster->callback(*broadcaster, *broadcaster->nodes[a], event.mods.rightButtonModifier);
					}
					else if (event.mods.isLeftButtonDown())
					{
						broadcaster->callback(*broadcaster, *broadcaster->nodes[a], event.mods.leftButtonModifier);
					}
					return;
				}
			}
		}

		virtual void mouseDrag(const MouseEvent& event)
		{
			Vector2i pos = event.getEventRelativeTo(broadcaster->getParentComponent()).getPosition();
			nodeClicked(pos, event);
		}
	private:
		Vector2i initialMouseDownPos;
		GridTab* broadcaster;
	};
	friend GridTabListener;
	GridTabListener listener;
public:
	virtual ~GridTab() 
	{
		removeMouseListener(&listener);
		removeKeyListener(&listener);
	}

	GridTab(Vector2i tSize, Vector2i nSize, std::function<void(GridTab&, ImageComponent&, ModifierKeys)> pCallback,
		OwnedArray<ImageComponent> pNodes = OwnedArray<ImageComponent>())
		: Tab(tSize), nodeSize(nSize), callback(pCallback), listener(this)
	{
		addMouseListener(&listener, true);
		addKeyListener(&listener);
		
		int size = tSize.x * tSize.y;
		if (!pNodes.isEmpty())
		{
			nodes = std::move(pNodes);
		}
		else
		{
			for (int a = 0; a < size; a++)
			{
				nodes.add(new ImageComponent(std::to_string(a)));
				nodes[a]->setImage(getDefaultNodeImage());
			}
		}
		for (int a = 0; a < size; a++)
		{
			addAndMakeVisible(*nodes[a]);
		}
		setSize(tSize.x * nSize.x, tSize.y * nSize.y);

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


	static const Image& getDefaultNodeImage()
	{
		static const Image defaultNodeImage = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("Asset/defaultNodeImage.jpg"));
		return defaultNodeImage;
	}
	OwnedArray<ImageComponent>& getNodes() { return nodes; }

	void fill(const Image& image)
	{
		for (int a = 0; a < nodes.size(); a++)
		{
			nodes[a]->setImage(image);
		}
	}
private:
	std::function<void(GridTab&, ImageComponent&, ModifierKeys)> callback;
	                              
	OwnedArray<ImageComponent> nodes; 
	Vector2i nodeSize;

	//  static const Image defaultNodeImage;
};




