#include "pch.h"
#include "timer.h"

void Timer::update()
{
	static DWORD _fps_tick = GetTickCount() + 1000;
	static int fpscount = 0;

	++fpscount;

	if (_fps_tick < GetTickCount())
	{
		_fps_tick = GetTickCount() + 1000;
		fps = fpscount;
		fpscount = 0;
	};

	for (auto event_iter = std::begin(_events); event_iter != std::end(_events);)
	{
		auto& [remain_time, function] = *event_iter;

		remain_time -= delta();

		if (remain_time < 0)
		{
			function();
			event_iter=_events.erase(event_iter);
		}
		else
		{
			++event_iter;
		}
	}
}

void Timer::render(HDC hdc)
{
	std::wstringstream wss;
	wss << "FPS : " << fps << "DeltaTime : " << dt << std::endl;

	RECT _rt{ 50,50,200,200 };

	DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
}

void Timer::initialize()
{
	tick = GetTickCount();
}

float Timer::delta()
{
	return dt /1000.f * time_scale;
}
