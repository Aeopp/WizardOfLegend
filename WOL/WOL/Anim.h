#pragma once
#include <array>
#include <utility>

struct Anim
{
public:
	bool bOn{ true };

	// 현재 작업 스프라이트 기준 로우인덱스는 애니메이션 모션자체를 의미
	int RowIndex{0};
	// 열은 해당 모션의 장면들중 하나를 의미
	int ColIndex{0};
	float AnimDuration{0};
	float CurDelta{0};
	// 인덱스에는 이미지의 행을 원소에는 이미지의 해당 행의 열의 개수
	std::vector<int> Anim_Info{};
	int DefaultClip{ 0 };

	bool bChangeable = true;
public:
	// 현재 애니메이션 재생이 끝날시 다음에 재생할 기본 애니메이션을 행번호로 지정해주세요.
	// 인덱스에는 이미지의 행을 원소에는 이미지의 해당 행의 열의 개수와 해당 행의 경로를 제외한 이미지 이름과 확장자를 매핑시켜주세요.
	void SetAnimationClip(std::vector<int> pAnim_Info, float AnimDuration);
	void AnimPlayLoop(int SetRowIndex);
	void AnimPlay(int SetRowIndex, float AnimDuration);
	void SetDefaultClip(int SetRowIndex);
	void update();
	inline int CurClipRowIndex()const
	{
		return RowIndex;
	}
};



