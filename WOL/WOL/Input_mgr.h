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
	void update() ;
	void initialize();

	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

	std::optional<vec> GetWindowMousePos()const&;
	std::optional<vec> GetWorldMousePos()const&;

	std::weak_ptr<class object> _Mouse{};

	template<typename ..._params>
	[[deprecated(L"아직 제대로 작동 안하니 사용 하지 말것")]] void Event_Regist(int _key, key_event _event, _params&&... params);
private:
	void Key_Update();
	std::array<bool,VK_MAX> m_bKeyState;
	// 해당 키 -> 버튼 프레스 이벤트가 무엇인지 -> 해당 함수
	// 왜인지 HOLD 빼고는 이벤트 노티파이가 안됨 아직 사용 하지 말자
	std::map<int, std::map<key_event, std::vector<std::function<void()>>>> _events;


	[[deprecated(L"아직 제대로 작동 안하니 사용 하지 말것")]] void Event_Update();
};

template<typename ..._params>
[[deprecated(L"아직 제대로 작동 안하니 사용 하지 말것")]] void Input_mgr::Event_Regist(int _key, key_event _event, _params&& ...params)
{
	 _events[_key][_event].emplace_back(std::bind(std::forward<_params>(params)...) );
}
