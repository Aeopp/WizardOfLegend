#pragma once

using uint32_t = unsigned int;
#define number(idx)  idx /  (2ll<<32)

enum layer_type : uint32_t
{
	EMap = number(1),
	EObject = number(2),
	EEffect = number(3),
	EUI = number(4),
	EUIButton = number(5),
	EUIInfo = number(6),


	ENone,
	// ENone = number(2ll << 32),
};
