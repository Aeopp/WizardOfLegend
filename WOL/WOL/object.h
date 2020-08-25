#pragma once
#include "pch.h"
#include "layer_type.h"
#include "transform.h"

class object abstract
{
public:
	virtual void update(float dt);
	virtual void render(HDC hdc,vec camera_pos);
	virtual void initialize();
	virtual void release();
	void late_initialize() {};
public:
	virtual uint32_t get_layer_id()const&;
	void set_ptr(std::weak_ptr<object> _ptr)&;
	std::weak_ptr<object> get_ptr()const{return _ptr;}
public:
	std::shared_ptr<Transform> _transform;
	std::weak_ptr<object> _ptr{};
	std::weak_ptr<object> _owner{};
	void Hit(std::weak_ptr<object> _target);
};

