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

    template<typename ..._params>
    void event_regist_ReWhileDelta(float RemainTime, float Delta, _params&&... params);

    std::list < std::pair<float, std::function<bool()>> > once_events;
    std::list <std::pair<float, std::function<bool()>>> rewhile_events;
    std::list <std::tuple<float, float, std::function<bool()>>> loop_events;
    //          전체 수행시간 , 각 수행당 시간 격차 , 함수
    std::list <std::tuple<float, float, float,std::function<void()>>> rewhileDT_events;

    int max_fps{1000};
    int fps{ 65 };

    float time_scale{ 1.0f };
    float dt{ 0.f };
    float tick{ 0.f };
private:
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

 template<typename ..._params>
 inline void Timer::event_regist_ReWhileDelta
 (float RemainTime, float Delta, _params&& ...params)
 {
     // 전체수행시간 , 원하는 시간격차 , 시간계산에 쓰일 변수
     rewhileDT_events.emplace_back(RemainTime, Delta, Delta,
      std::bind(std::forward<_params>(params)...));
 }

