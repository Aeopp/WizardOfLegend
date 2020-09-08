#pragma once
#include <memory>
#include "object.h"
#include <queue>

// 이벤트 존 사이즈를 설정해주세요
// 트랜스폼의 로케이션으로 이벤트 존의 위치를 설정해주세요.
// 마지막 이벤트를 제외한 이벤트는 오브젝트를 생성시킨 이후 전부 리턴시켜주세요.
class Trigger : public object
{
public:	
	Event update(float dt)override;
	void initialize()override;
	void Hit(std::weak_ptr<object> _target)override;
	// 플레이어가 이벤트 발생 구역에 입장했는지 확인
	std::pair<int, int> EventZoneSize{};

	void SetUp(std::pair<int,int> EventZoneSize,vec Location , 
	std::function<std::vector<std::weak_ptr<class object>>()> StartEvent,
	std::function<void()> EndEvent,
	std::queue<std::function<std::vector<std::weak_ptr<class object>>()>>
		EventList);

	bool bEventEnd = false;
private:
	bool bEventStart = false;
	
	std::vector<std::weak_ptr<class object>> wp_EventZoneMonsters;
	std::weak_ptr<class collision_component> wp_collision;
	std::function<std::vector<std::weak_ptr<class object>>()> Event_Start{};
	std::queue<std::function<std::vector<std::weak_ptr<class object>>()>> EventQ;
	std::function<void()> Event_End{};
	std::vector<std::weak_ptr<class Prison>> PrisonList;

	bool ConditionCheck();
};



