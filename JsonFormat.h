/*
  ==============================================================================

    JsonFormat.h
    Created: 18 Jun 2017 11:08:05am
    Author:  Lorence

  ==============================================================================
*/

#pragma once
#include <string>
#include <vector>
class JsonFormat
{
	std::string map_column = "column";
	std::string map_row = "row";
	std::string orientation = "orientation";
	std::string renderorder = "renderorder";
	std::string tile_height = "tile_height";
	std::string tile_width = "tile_width";
	std::string width = "width";
	std::string height = "height";
	std::string tiles = "tiles"; //array


	std::string tilesets = "tilesets";
	//{
	std::string tileset_column = "column";
	std::string file = "file";
	std::string image_height = "image_height";
	std::string image_width = "image_width";
	std::string name = "name";
	std::string properties = "properties"; //array
	std::string tileset_row = "row";
	std::string tiles_count = "tile_count";
	std::string tile_height = "tile_height";
	std::string tile_width = "tile_width";
	//}
};