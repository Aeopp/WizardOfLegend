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

class Input_mgr : public singleton_interface<Input_mgr>
{
public:
	void update() override;
	void initialize()override;

	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

	template<typename ..._params>
	void Event_Regist(int _key, key_event _event, std::function<void()> func, _params&&... params);
private:
	void Key_Update();
	std::array<bool,VK_MAX> m_bKeyState;
	// 해당 키 -> 버튼 프레스 이벤트가 무엇인지 -> 해당 함수
	std::map<int, std::map<key_event, std::vector<std::function<void()>>>> _events;
};

template<typename ..._params>
 void Input_mgr::Event_Regist(int _key, key_event _event, std::function<void()> func, _params&& ...params)
{
	 _events[_key][_event].emplace_back(std::bind(func, std::forward<_params>(params)...));
}
