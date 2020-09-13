#include "pch.h"
#include "Trigger.h"
#include "collision_component.h"
#include "collision_mgr.h"
#include "Prison.h"
#include "object_mgr.h"
#include "Camera.h"

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

			release();
			return Event::Die;
		}
	}
	//cheat key
	if(GetAsyncKeyState('0')&0x8000)
	{
		for(auto& DieObj : wp_EventZoneMonsters)
		{
			auto sp_DieObj =DieObj.lock();
			if (!sp_DieObj)continue;
			sp_DieObj->bDie = true;
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

		TriggerStart();
	}
}

void Trigger::release() noexcept
{
	bEventEnd = true;
	bDie = true;

	if (!bCameraFixZone)return;

	auto sp_camera = object_mgr::instance()._Camera.lock();
	if (!sp_camera)return;
	sp_camera->SetCameraFix(false);
}
 
 /// </summary>
 /// <param name="EventZoneSize"></param>
 /// Ʈ���� �̺�Ʈ ������ �����ϴ� ����
 /// <param name="Location"></param>
 /// Ʈ���� ���� ��ġ
 /// <param name="StartEvent"></param>
 /// �������� ���� �̺�Ʈ Prison ������ �ʿ��Ѱ�� ���⼭ �������ּ���.
 /// <param name="EndEvent"></param>
 /// �÷��̾ ���������� Ŭ���� ������� �̺�Ʈ
 /// <param name="EventList"></param>
 /// ���͸� �����ϰ� ���翩�θ� �����ؼ� ���� �̺�Ʈ�� �Ѿ�� �����մϴ�.
 /// �����ؾ��ϴ� ���͵��� ��ȯ���ּ���.
 /// <param name="bCameraFixZone"></param>
 /// ī�޶� ��ġ�� ���� �� ������ ������ּ��� �⺻�� false
 /// <param name="CameraRange"></param>
 /// ī�޶� ��ġ�� �����Ұ�� ��ġ ������ �Ѱ��ּ���.
 void Trigger::SetUp
 (std::pair<int, int> EventZoneSize, vec Location,
	 std::function<std::vector<std::weak_ptr<class object>>()> StartEvent, 
	 std::function<void()> EndEvent, 
	 std::queue<std::function<std::vector<std::weak_ptr<class object>>()>> EventList,
	 bool bCameraFixZone , std::pair<vec, vec> CameraRange)
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

	
	if (!bCameraFixZone)return;
	this->bCameraFixZone = bCameraFixZone;
	TriggerCameraRange = std::move(CameraRange);
}

 void Trigger::TriggerStart()
 {
	 if (!bCameraFixZone)return;

	 auto sp_camera = object_mgr::instance()._Camera.lock();
	 if (!sp_camera)return;
	 sp_camera->SetCameraFix(bCameraFixZone, TriggerCameraRange);
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
