#pragma once
#include "singleton_interface.h"

enum time_event : uint8_t
{
    EOnce,
    ELoop,
    ERemaingWhile,
};
class Timer :
    public singleton_interface<Timer>
{
public:
    void update();
    void render(HDC hdc);
    void initialize();

    float delta();
    template<typename ..._params>
    void event_regist(time_event _event, float time, _params&&... params);

    std::list < std::pair<float, std::function<void()>> > once_events;
    std::list <std::pair<float, std::function<void()>>> rewhile_events;
    std::list <std::tuple<float, float, std::function<bool()>>> loop_events;

    int max_fps{1000};
    int fps{ 0};

    float time_scale{ 1.f };
    float dt{ 0.f };
    float tick{ 0.f };
};

 template<typename ..._params>
 void Timer::event_regist(time_event _event, float time,_params&& ...params)
 {
     switch (_event)
     {
     case EOnce:
         once_events.emplace_back(time, std::bind(std::forward<_params>(params)...));
         break;
     case ELoop:
         loop_events.emplace_back(time,time, std::bind(std::forward<_params>(params)...));
         break;
     case ERemaingWhile:
         rewhile_events.emplace_back(time, std::bind(std::forward<_params>(params)...));
         break;
     default:
         break;
     }
 }

