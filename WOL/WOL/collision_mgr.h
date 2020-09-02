#pragma once
#include "pch.h"
#include "singleton_interface.h"
#include "collision_component.h"

class collision_mgr : public singleton_interface<collision_mgr>
{
public:
	void collision_tile_clear();
	// 타일은 부동 Rhs 를 밀어버립니다.
	void collision_tile(collision_tag rhs);
	std::vector<std::pair<int, int>> _Tile_Collision_List;
	void Insert_CollisionTile(std::pair<int, int> _location);
	void Erase_CollisionTile(std::pair<int, int> _location);

	void save_collision(std::wstring filename);
	void load_collision(std::wstring filename );

	std::map<collision_tag, std::list<std::shared_ptr<collision_component>>> _collision_map;

	std::weak_ptr< collision_component> insert(std::weak_ptr<class object> _owner,collision_tag _tag,
		figure_type _type = ERect);

	std::shared_ptr<class Bmp> HitEffectImg{};


	// 이펙트 위치 컬럼 로우 델타
	std::list<std::tuple<vec, int,int,float,float>> CollisionHitEffectList{};

	void collision(collision_tag lhs, collision_tag rhs);
	void render(HDC hdc, std::pair<float, float>  size_factor) ;
	void update();
	void release();

	static inline const std::wstring DefaultMapCollisionPath = 
		L"..\\..\\Resources\\CollisionInfo\\";
	static inline const std::wstring StageFileName=
		L"InfoStage.txt";
	static inline const std::wstring BossStageFileName =
		L"InfoBoss.txt";

	bool IsHitEffectMappingTag(collision_tag lhs,collision_tag rhs);
	bool IsObjectSlideMappingTag(collision_tag lhs, collision_tag rhs);

	void HitEffectPush(vec location, float Duration);
private:
	void check_erase()&;
	int CollisionRangeX;
	int CollisionRangeY;
	int CollisionTileNum{};

	int CollisionObjNum{};

	float TileHitEffectDuration = 0.2f;
};

