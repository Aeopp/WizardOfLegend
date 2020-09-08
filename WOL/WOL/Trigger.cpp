#include "pch.h"
#include "Trigger.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Prison.h"

Event Trigger::update(float dt)
{
	Event _Event = object::update(dt);

	if (!bEventStart)return _Event;

	// ���� �̺�Ʈ�� �߻���Ŵ
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
		// �̺�Ʈ�� �� ����Ǿ���.
		else
		{
			// ������ �̺�Ʈ �����ϰ� Ʈ���Ŵ� ����
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
	// �̺�Ʈ �߻� ���� �浹 �˻�� ���ʿ�
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
	// ���� Ʈ���Ű� �����ϴ� ������Ʈ�� ���� �Ҹ�Ǿ��ٸ� ���� �̺�Ʈ
	return std::all_of(std::begin(wp_EventZoneMonsters),
	                   std::end(wp_EventZoneMonsters),IsMonsterDie);
}
