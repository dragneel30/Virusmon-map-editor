/*
  ==============================================================================

    Tile.h
    Created: 18 May 2017 5:49:13am
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Properties.h"
#include <string>



template<typename T>
class UID
{
public:
	UID(const T& iid)
		: id(iid)
	{

	}

	UID(const UID& iid)
		: id(iid.getID())
	{

	}
	const T& getID() const { return id; }
	UID& operator+(const T& val)
	{
		id += val;
		return *this;
	}
	UID operator++(int)
	{
		UID temp = *this;
		id++;
		return temp;
	}
	bool operator!=(T iid)
	{
		return id != iid;
	}
private:
	UID& operator=(UID& newID);
	T id;
};

typedef UID<int> intUID;
typedef UID<std::string> strUID;


class TileType
{
public:
	TileType(intUID iid, const Image& pImage)
		: image(pImage), id(iid)
	{
		strProperties.add("ID", std::to_string(iid.getID()));
		strProperties.add("sizeX", "1");
		strProperties.add("sizeY", "1");
	}
	TileType(const TileType& other)
		: TileType(other.getID(), other.getImage())
	{

	}

	Properties& getStrProperties() { return strProperties; }
	const Image& getImage() const { return image; }
    intUID getID() const { return id; }
private:
	intUID id;
	Image image;
	Properties strProperties;
};

class Tile : public ImageComponent
{
public:
	Tile(std::shared_ptr<TileType> type)
		: Tile()
	{
		setSharedProperties(type);
	}
	Tile()
		: sharedProperties(nullptr)
	{

	}
	void pointTo(Tile* tile)
	{
		setSharedProperties(tile->getSharedProperties());
	}

	~Tile()
	{
	}

	void setSharedProperties(std::shared_ptr<TileType> type)
	{ 
		sharedProperties = type; 
		setImage(type->getImage());
	}

	std::shared_ptr<TileType> getSharedProperties() const { return sharedProperties; }
	
private:
	std::shared_ptr<TileType> sharedProperties;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tile)
};