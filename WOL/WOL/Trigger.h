#pragma once
#include <memory>
#include "object.h"
#include <queue>

// �̺�Ʈ �� ����� �������ּ���
// Ʈ�������� �����̼����� �̺�Ʈ ���� ��ġ�� �������ּ���.
// ������ �̺�Ʈ�� ������ �̺�Ʈ�� ������Ʈ�� ������Ų ���� ���� ���Ͻ����ּ���.
class Trigger : public object
{
public:	
	Event update(float dt)override;
	void initialize()override;
	void Hit(std::weak_ptr<object> _target)override;
	// �÷��̾ �̺�Ʈ �߻� ������ �����ߴ��� Ȯ��
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



