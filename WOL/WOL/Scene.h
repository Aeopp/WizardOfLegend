#pragma once
#include <utility>
#include <windef.h>

class Scene
{
public:
	virtual ~Scene()noexcept;
public:
	virtual void render(HDC hdc, std::pair<float, float> size_factor) {};
	virtual void update(float dt) {};
	virtual void initialize() {};
	virtual void release();
public:
	std::vector<std::shared_ptr<class object>>manage_objs;
};

