#include "pch.h"
#include "Anim.h"
#include "Bmp_mgr.h"

void Anim::SetAnimationClip(std::vector<int>pAnim_Info, float AnimDuration)
{
	this->Anim_Info = std::move(pAnim_Info);
	this->AnimDuration = AnimDuration;
}

 void Anim::AnimPlayLoop(int SetRowIndex)
{
	 if (!bChangeable)return;
	RowIndex = DefaultClip = SetRowIndex;
}

void Anim::AnimPlay(int SetRowIndex, float AnimDuration)
{
	 if (RowIndex == SetRowIndex)return;
	 if (!bChangeable)return;

	RowIndex = SetRowIndex;
	ColIndex = 0;
	this->AnimDuration = AnimDuration;
}

 void Anim::SetDefaultClip(int SetRowIndex)
{
	DefaultClip = SetRowIndex;
}

void Anim::update()
{
	if (!bOn)return;
	if (Anim_Info.empty())return;

	CurDelta -= DeltaTime;
	if (CurDelta < 0)
	{
		CurDelta = (AnimDuration/(float)Anim_Info[RowIndex]);
		++ColIndex;
		if (ColIndex >= Anim_Info[RowIndex])
		{
			ColIndex = 0;
			RowIndex = DefaultClip;
		}
	}
}

