#pragma once
#include "ICE_Blast.h"

class Freezing_Interface abstract
{
public:
	vec Freez_size = { 200,200 };
	void Freezing_update(float dt,std::weak_ptr<class collision_component> 
	_collision_component);
	/// <param name="location"></param>
	/// ī�޶� ��ǥ���� �� ������ ȭ�� ��ǥ�� �н����ּ���.
	void Freezing_render(HDC hdc, vec location);
public:
	ICE_Blast::Freezing _Freezing_Info;
};

