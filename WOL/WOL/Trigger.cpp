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
	// 이벤트 발생 이후 충돌 검사는 불필요
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
 /// 트리거 이벤트 시작을 감지하는 범위
 /// <param name="Location"></param>
 /// 트리거 존의 위치
 /// <param name="StartEvent"></param>
 /// 스테이지 입장 이벤트 Prison 생성이 필요한경우 여기서 생성해주세요.
 /// <param name="EndEvent"></param>
 /// 플레이어가 스테이지를 클리어 했을경우 이벤트
 /// <param name="EventList"></param>
 /// 몬스터를 생성하고 생사여부를 추적해서 다음 이벤트로 넘어갈지 결정합니다.
 /// 추적해야하는 몬스터들을 반환해주세요.
 /// <param name="bCameraFixZone"></param>
 /// 카메라 위치를 제한 할 것인지 명시해주세요 기본값 false
 /// <param name="CameraRange"></param>
 /// 카메라 위치를 제한할경우 위치 범위를 넘겨주세요.
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
	// 현재 트리거가 관리하는 오브젝트가 전부 소멸되었다면 다음 이벤트
	return std::all_of(std::begin(wp_EventZoneMonsters),
	                   std::end(wp_EventZoneMonsters),IsMonsterDie);
}
