#pragma once
#include <array>
#include <utility>

struct Anim
{
public:
	bool bOn{ true };

	// ���� �۾� ��������Ʈ ���� �ο��ε����� �ִϸ��̼� �����ü�� �ǹ�
	int RowIndex{0};
	// ���� �ش� ����� ������ �ϳ��� �ǹ�
	int ColIndex{0};
	float ImageDelta{0};
	float CurDelta{0};
	// �ε������� �̹����� ���� ���ҿ��� �̹����� �ش� ���� ���� ������ �ش� ���� ��θ� ������ �̹��� �̸��� Ȯ���ڸ� ���ν����ּ���.
	std::vector<int> Anim_Info{};
	int DefaultClip{ 0 };
public:
	// ���� �ִϸ��̼� ����� ������ ������ ����� �⺻ �ִϸ��̼��� ���ȣ�� �������ּ���.
	// �ε������� �̹����� ���� ���ҿ��� �̹����� �ش� ���� ���� ������ �ش� ���� ��θ� ������ �̹��� �̸��� Ȯ���ڸ� ���ν����ּ���.
	void SetAnimationClip(std::vector<int> pAnim_Info, float ImageDelta);
	void AnimPlayLoop(int SetRowIndex);
	void AnimPlay(int SetRowIndex, int ImageDelta);
	void SetDefaultClip(int SetRowIndex);
	void update();
	inline int CurClipRowIndex()const
	{
		return RowIndex;
	}
};


