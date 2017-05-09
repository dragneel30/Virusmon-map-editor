

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
protected:
	Vector2i tabSize;
};

class GridTab : public Tab
{
public:
	GridTab(Vector2i tSize, Vector2i nSize,  OwnedArray<ImageComponent> pNodes = OwnedArray<ImageComponent>())
		: Tab(tSize), nodeSize(nSize)
	{
		if ( !pNodes.isEmpty() )
		{
			nodes = std::move(pNodes);
		}
		else
		{
			int size = tSize.x * tSize.y;
			for (int a = 0; a < size; a++)
			{
				nodes.add(new ImageComponent(std::to_string(a)));
				nodes[a]->setImage(getDefaultNodeImage());
			}
		}
		for (int a = 0; a < nodes.size(); a++)
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
				myLog(std::to_string(x) + " " + std::to_string(y));
				x = 0;
				y += nodeSize.y;
			}
		}
	}

private:
	OwnedArray<ImageComponent> nodes; 
     //  static const Image defaultNodeImage;
	static const Image& getDefaultNodeImage()
	{
		static const Image defaultNodeImage = ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("Asset/defaultNodeImage.jpg"));
		return defaultNodeImage;
	}
	Vector2i nodeSize;
};

