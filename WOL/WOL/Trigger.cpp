#include "pch.h"
#include "Trigger.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Prison.h"

Event Trigger::update(float dt)
{
	Event _Event = object::update(dt);

	if (!bEventStart)return _Event;

	// 다음 이벤트를 발생시킴
 	if (ConditionCheck())
	{
		wp_EventZoneMonsters.clear();

		if (!EventQ.empty())
		{
			auto& Event = EventQ.front();

			if (Event)
				wp_EventZoneMonsters = std::move(Event());

			EventQ.pop();
		}
		// 이벤트가 다 종료되었다.
		else
		{
			// 마지막 이벤트 실행하고 트리거는 삭제
			if (Event_End)
				Event_End();

			bEventEnd = true;
			bDie = true;
			return Event::Die;
		}
	}

	return _Event; 
}

void Trigger::initialize()
{
    object::initialize();

	wp_collision = collision_mgr::instance().
		insert(_ptr, collision_tag::ETrigger, figure_type::ERect);

	auto sp_collision = wp_collision.lock();
	if (!sp_collision) return;

	sp_collision->bCollision = true;
	sp_collision->bCollisionSlideAnObject = false;
	sp_collision->bCollisionTargetPushFromForce = false;
	sp_collision->bHitEffect = false;
	sp_collision->bRender = true;
	sp_collision->bSlide = false;
	sp_collision->PushForce = 0.f;
	sp_collision->_size = EventZoneSize;
}

void Trigger::Hit(std::weak_ptr<object> _target)
{
    object::Hit(_target);

	if (bEventStart)return;
	// 이벤트 발생 이후 충돌 검사는 불필요
	auto sp_collision = wp_collision.lock();
	if (!sp_collision) return;
	sp_collision->bDie = true;
	
	if (Event_Start)
	{
		wp_EventZoneMonsters = Event_Start();
		bEventStart = true;
	}
}

 void Trigger::SetUp(std::pair<int, int> EventZoneSize, vec Location, std::function<std::vector<std::weak_ptr<class object>>()> StartEvent, std::function<void()> EndEvent, std::queue<std::function<std::vector<std::weak_ptr<class object>>()>> EventList)
{
	this->EventZoneSize = std::move(EventZoneSize);
	if (!_transform)return;
	_transform->_location = std::move(Location);
	this->Event_Start = std::move(StartEvent);
	this->Event_End = std::move(EndEvent);
	this->EventQ = std::move(EventList);

	auto sp_collision = wp_collision.lock();
	if (!sp_collision) return;
	sp_collision->_size = EventZoneSize;
}


bool Trigger::ConditionCheck()
{
	static auto IsMonsterDie = [](std::weak_ptr<class object>wp_Monster)
	->bool 
	{
		auto sp_Monster = wp_Monster.lock();
		return !sp_Monster || sp_Monster->bDie;
	};
	// 현재 트리거가 관리하는 오브젝트가 전부 소멸되었다면 다음 이벤트
	return std::all_of(std::begin(wp_EventZoneMonsters),
	                   std::end(wp_EventZoneMonsters),IsMonsterDie);
}
