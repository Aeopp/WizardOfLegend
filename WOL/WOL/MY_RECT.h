#pragma once
#include <wtypes.h>


template<typename T>
RECT make_rect(const T left, const T Top, const T Right, const T Bottom)
{
	return RECT{ (LONG)left,(LONG)Top,(LONG)Right,(LONG)Bottom };
}

template<typename T>
POINT make_pt(const T x, const T y)
{
	return POINT{ LONG(x),LONG(y) };

}
