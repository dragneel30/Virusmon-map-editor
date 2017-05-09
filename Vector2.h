/*
  ==============================================================================

    Vector2.h
    Created: 30 Apr 2017 2:24:15pm
    Author:  Lorence

  ==============================================================================
*/

#pragma once

template<class T> struct Vector2
{
	Vector2(T x, T y)
		: x(x), y(y)
	{

	}
	T x;
	T y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
