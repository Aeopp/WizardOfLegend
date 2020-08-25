#pragma once
#include "singleton_interface.h"
class Timer :
    public singleton_interface<Timer>
{
public:
    void update()override;
    void render(HDC hdc)override;
    void initialize()override;

    float delta();
    template<typename ..._params>
    void event_regist(float time,std::function<void()> event, _params&&... params);

    std::list < std::pair<float, std::function<void()>>  > _events;

    int max_fps{ 65 };
    int fps{ 0};

    float time_scale{ 1.f };
    float dt{ 0.f };
    float tick{ 0.f };
};

template<typename ..._params>
 void Timer::event_regist(float time,std::function<void()> event, _params&& ...params)
{
     _events.emplace_back(time,std::bind(event, std::forward<_params>(params)...) );
}
