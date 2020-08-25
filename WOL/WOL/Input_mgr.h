#pragma once
#include "pch.h"
#include "singleton_interface.h"

#define VK_MAX 0xff
enum key_event
{
	EUp,
	EDown,
	EHold,
};

class Input_mgr : singleton_interface<Input_mgr>
{
public:
	void update() override ;
	void initialize()override  ;

	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);
	
private:
	void Key_Update();

	std::array<bool,VK_MAX> m_bKeyState;
	// �ش� Ű -> ��ư ������ �̺�Ʈ�� �������� -> �ش� �Լ�
	std::map<int, std::map<key_event, std::vector<std::function<void()>>>> _events;
};


