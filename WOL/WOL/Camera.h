#pragma once
#include "object.h"

class Camera : public  object
{
public:
	 Event update(float dt)override;
	 void initialize()override;

	class object_mgr* _object_mgr{ nullptr };
};

