#pragma once
#include "pch.h"
#include "layer_type.h"
#include "transform.h"
#include "event.h"

class object abstract
{
public:
	virtual Event update(float dt);
	virtual void render(HDC hdc, vec camera_pos, vec size_factor);
	virtual void initialize();
	virtual void release();
	void late_initialize() {};
public:
	virtual uint32_t get_layer_id()const&;
	void set_ptr(std::weak_ptr<object> _ptr)&;
	std::weak_ptr<object> get_ptr()const{return _ptr;}
public:
	std::shared_ptr<Transform> _transform{};
	std::weak_ptr<object> _ptr{};
	std::weak_ptr<object> _owner{};

	virtual void Hit(std::weak_ptr<object> _target);
	virtual void HitTile(RECT TileRt);

	bool bDie{ false };
};

