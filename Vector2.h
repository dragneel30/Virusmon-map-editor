/*
  ==============================================================================

    Vector2.h
    Created: 30 Apr 2017 2:24:15pm
    Author:  Lorence

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode//JuceHeader.h"
template<class T> struct Vector2
{
	Vector2(T x, T y)
		: x(x), y(y)
	{

	}
	Vector2(Point<T> p)
		: Vector2(p.x, p.y)
	{
		
	}
	
	Vector2<T> operator=(Point<T> p)
	{
		x = p.x;
		y = p.y;
		return *this;
	}

	bool operator==(Vector2<T> v)
	{
		return (v.x == p.x && v.y == p.y);
	}
	T x;
	T y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
