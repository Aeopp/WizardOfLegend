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
	[[deprecated(L"���� ����� �۵� ���ϴ� ��� ���� ����")]] void Event_Regist(int _key, key_event _event, _params&&... params);
private:
	void Key_Update();
	std::array<bool,VK_MAX> m_bKeyState;
	// �ش� Ű -> ��ư ������ �̺�Ʈ�� �������� -> �ش� �Լ�
	// ������ HOLD ����� �̺�Ʈ ��Ƽ���̰� �ȵ� ���� ��� ���� ����
	std::map<int, std::map<key_event, std::vector<std::function<void()>>>> _events;


	[[deprecated(L"���� ����� �۵� ���ϴ� ��� ���� ����")]] void Event_Update();
};

template<typename ..._params>
[[deprecated(L"���� ����� �۵� ���ϴ� ��� ���� ����")]] void Input_mgr::Event_Regist(int _key, key_event _event, _params&& ...params)
{
	 _events[_key][_event].emplace_back(std::bind(std::forward<_params>(params)...) );
}
