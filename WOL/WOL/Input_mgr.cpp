#include "pch.h"
#include "Input_mgr.h"
#include "object.h"
#include "object_mgr.h"

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

std::optional<vec> Input_mgr::GetWindowMousePos() const&
{
	auto PMouse = _Mouse.lock();
	if (!PMouse)return std::nullopt;
	
	return std::optional<vec>(PMouse->_transform->_location);
}

std::optional<vec> Input_mgr::GetWorldMousePos() const&
{
	auto OMouse = GetWindowMousePos();
	if (!OMouse) return std::nullopt;

	return *OMouse + object_mgr::instance().camera_pos;
};

void Input_mgr::Key_Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
		if (!m_bKeyState[i] && (GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}
}


[[deprecated(L"아직 제대로 작동 안하니 사용 하지 말것")]]
void Input_mgr::Event_Update()
{
#pragma region TODO::DEBUG
	{
		for (auto& [key, Event] : _events)
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

				if (bExecution)
				{
					std::for_each(std::begin(function), std::end(function), [](std::function<void()>& func) {func(); });
				}
			}
		}
	}
#pragma endregion TODO::DEBUG
};

void Input_mgr::update()
{
	if (Key_Down('P'))
		bDebug = !bDebug;
	if (Key_Down('O'))
		bFrameLimit = !bFrameLimit;
	if (Key_Down('I'))
		bDeltaTimeUnFixed = !bDeltaTimeUnFixed;
	if (Key_Down('U'))
		bDebugTimeScale= !bDebugTimeScale;
	Key_Update();

	// Event_Update();
};

void Input_mgr::initialize()
{
	m_bKeyState.fill(NULL);
};
