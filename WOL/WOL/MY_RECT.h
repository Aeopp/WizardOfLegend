#pragma once
#include <wtypes.h>


template<typename T>
RECT make_rect(const T left, const T Top, const T Right, const T Bottom)
{
	return RECT{ (LONG)left,(LONG)Top,(LONG)Right,(LONG)Bottom };
}