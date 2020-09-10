#pragma once
#include <unordered_map>
#include <queue>
#include "object.h"


class FireDragon : public object 
{
public:	
	  void initialize()override;
	  Event update(float dt)override;
	  std::weak_ptr<class collision_component>  _collision_component;
	  uint32_t get_layer_id()const& override;
	  void render(HDC hdc, vec camera_pos, vec size_factor)override;
	  void HitTile(RECT TileRt)override;

	  void SetUp(vec Location,vec Dir);
public:
	bool bWallHited = false;
	std::shared_ptr<class Bmp> sp_DragonImg;
	std::shared_ptr<class Bmp> sp_ParticleImg;
	static inline float factor = 1.f;
	  float Duration;
	  int PaintSizeX;
	  int PaintSizeY;
	  float Speed;
	  float Scale; 
	  float P_Scale;

	  int ColIdx;
	  int RowIdx;
	  float RotationSpeed;
	  float CurrentAngle; 
	  float AngleMax;
	  float AngleMin;
	  float MyFactor;
	  float ParticleLocationUpdateDelta = 0.02f;
	  float ParticleDelta = ParticleLocationUpdateDelta; 
	  float ParticelAnimDelta = 0.035f;

	  int ParticleMaxNum;

	  std::deque<std::tuple<vec,int,int,float>> ParticleLocationDQ;


	  vec LineDir;
	  vec LinePos;
	  float amplitude;
	  float T;
private:
	void CalcSpriteFromAngle();
	/// <summary>
	/// first left second right
    //  key is 360Angle Divide by 19 value is FireDragon Sprite ColIdx
	/// </summary>
	std::pair<std::map<uint32_t, uint32_t>, std::map<uint32_t, uint32_t>> SpriteAngleColMap
	{
		{
			{7,0},{6,1},{5,2},{4,3},{8,7},
			{9,8},{10,9},{11,10},{12,11},
			{13,12},{14,13},{15,14},
			{16,15},{17,16},{18,17},{19,18}
		},
		{
			{0,9},{19,9 } ,{1,10},{2,11},{3,12},
		    {4,13},{5,14},{6,15},{18,8},
		    {17,7},{16,6},{15,5},{14,4},{13,3},
		    {12,6},{11,5},{10,4},{20,11}
		}
	};
};