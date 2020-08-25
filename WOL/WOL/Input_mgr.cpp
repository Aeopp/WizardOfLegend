#include "pch.h"
#include "Input_mgr.h"

bool Input_mgr::Key_Pressing(int _key)
{
	if (GetAsyncKeyState(_key) & 0x8000)
		return true;
	return false;
};

bool Input_mgr::Key_Down(int _key)
{
	if (!m_bKeyState[_key] && (GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

bool Input_mgr::Key_Up(int _key)
{
	if (m_bKeyState[_key] && !(GetAsyncKeyState(_key) & 0x8000))
	{
		m_bKeyState[_key] = !m_bKeyState[_key];
		return true;
	}
	return false;
}

void Input_mgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
};

void Input_mgr::update()
{
	Key_Update();

	for (auto& [key,Event] : _events)
	{
		for (auto& [_press, function] : Event)
		{
			bool bExecution = false;

			switch (_press)
			{
			case EUp:
				bExecution = Key_Up(key);
				break;
			case EDown:
				bExecution = Key_Down(key);
				break;
			case EHold:
				bExecution = Key_Pressing(key);
				break;
			default:
				break;
			}

			if(bExecution)
				std::for_each(std::begin(function), std::end(function), [](std::function<void()>& func) {func(); });
		}
	}
};

void Input_mgr::initialize()
{
	m_bKeyState.fill(NULL);
};
