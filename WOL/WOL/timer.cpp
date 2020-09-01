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

	for (auto event_iter = std::begin(once_events); event_iter != std::end(once_events);)
	{
		auto& [remain_time, function] = *event_iter;

		remain_time -= delta();

		if (remain_time < 0)
		{
			function();
			event_iter= once_events.erase(event_iter);
		}
		else
		{
			++event_iter;
		}
	}

	for (auto event_iter = std::begin(loop_events); event_iter != std::end(loop_events);)
	{
		auto& [remain_time,default_time, function] = *event_iter;

		remain_time -= delta();

		if (remain_time < 0)
		{
			remain_time = default_time;
			bool bEnd = function();

			if (bEnd) {
				event_iter = loop_events.erase(event_iter);
			}
			else {
				++event_iter;
			}
		}
		else
		{
			++event_iter;
		}
	}


	for (auto event_iter = std::begin(rewhile_events); event_iter != std::end(rewhile_events);)
	{
		auto& [remain_time, function] = *event_iter;

		remain_time -= delta();

		if (remain_time < 0)
		{
			event_iter = rewhile_events.erase(event_iter);
		}
		else
		{
			function();
			++event_iter;
		}
	}

}

void Timer::render(HDC hdc)
{
	if (bDebug)
	{
		std::wstringstream wss;
 		wss << "FPS : " << fps << "DeltaTime : " << dt << std::endl;

		RECT _rt{ 0,200,1600,300 };

		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
	}
}

void Timer::initialize()
{
	tick = GetTickCount();
}

float Timer::delta()
{
    return (1000.f / fps) * 0.001 * time_scale;
	//return 0.016f;

	//return dt /1000.f * time_scale;
}
