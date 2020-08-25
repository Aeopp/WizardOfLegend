#pragma once
#include <numeric>

using uint32_t = unsigned int;
#define number(idx)  idx /  (2ll<<32)

enum layer_type : uint32_t
{
	map = number(1),
	object = number(2),
	effect = number(3),
	UI = number(4),
	none  = number(2ll<<32),
};
