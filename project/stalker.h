#pragma once
#include "monster.h"

class stalker : public monster
{
private:
	using super = monster; 
public:
	virtual void update()override;
	virtual void initialize()override;

	GETSET(std::weak_ptr<class object>, _target);
};

