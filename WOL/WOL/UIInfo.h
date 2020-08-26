#pragma once
#include "UI.h"

class UIInfo : public UI
{
public:
	uint32_t get_layer_id() const& override;

};

