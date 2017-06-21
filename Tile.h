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
	bool operator >= (T iid)
	{

	}
	String toString()
	{
		return String(std::to_string(id));
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
		strProperties.add("width", "1");
		strProperties.add("height", "1");
		strProperties.setDefaultPropertiesCount(3);
		strProperties.setEdited(false);
	}
	TileType(const TileType& other)
		: TileType(other.getID(), other.getImage())
	{
		
	}

	Properties& getStrProperties() { return strProperties; }
	const Image& getImage() const { return image; }
	intUID getID() const { return intUID(std::stoi(strProperties.getProperty(0).toStdString())); }
private:
	intUID id; // will delete it
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
	Tile(Tile* tile)
	{
		pointTo(tile);
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