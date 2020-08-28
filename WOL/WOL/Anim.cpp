#include "pch.h"
#include "Anim.h"
#include "Bmp_mgr.h"

void Anim::SetAnimationClip(std::vector<int>pAnim_Info, float ImageDelta)
{
	this->Anim_Info = std::move(pAnim_Info);
	this->ImageDelta = ImageDelta;
}

 void Anim::AnimPlayLoop(int SetRowIndex)
{
	RowIndex = DefaultClip = SetRowIndex;
}

void Anim::AnimPlay(int SetRowIndex,int ImageDelta)
{
	 if (RowIndex == SetRowIndex)return;

	RowIndex = SetRowIndex;
	ColIndex = 0;
	CurDelta = ImageDelta;
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
		CurDelta = ImageDelta;
		++ColIndex;
		if (ColIndex >= Anim_Info[RowIndex])
		{
			ColIndex = 0;
			RowIndex = DefaultClip;
		}
	}
}

