#pragma once
#include "singleton_interface.h"
#include "collision_component.h"
#include "object.h"

#include <vector>
#include <map>
#include <unordered_map>

enum collision_tag : uint8_t
{
	player,
	monster,
};

class collision_mgr : public singleton_interface<collision_mgr>
{
public:
	using container_type = std::unordered_map<collision_tag,
	std::vector<std::shared_ptr<collision_component>>>;

	GETSET(container_type, _collisions);
	virtual void render(HDC hdc)override;

	virtual void update()override;
	virtual void release()override;
private:
};

