#include "pch.h"
#include "collision_mgr.h"
#include "object.h"
#include "object_mgr.h"
#include "game.h"
#include "Debuger.h"
#include <fstream>
#include <istream>
#include <ostream>
#include "math.h"
#include "Bmp_mgr.h"
#include "Bmp.h"
#include "helper.h"

#include "Font.h"


void collision_mgr::collision_tile_clear()
{
	if(!_Tile_Collision_List.empty())
	_Tile_Collision_List.clear();
};

void collision_mgr::collision_tile(collision_tag rhs)
{

	auto& rhs_list = _collision_map[rhs];

	// 임의의 범위내에서만 충돌검사를 수행한다.
	RECT CameraRange = game::client_rect;
	CameraRange.left -= CollisionRangeX;
	CameraRange.top -= CollisionRangeY;
	CameraRange.right += CollisionRangeX;
	CameraRange.bottom += CollisionRangeY;

	vec cp =object_mgr::instance().camera_pos;
	
	int x = game::TileWorldX / 2;
	int y = game::TileWorldY / 2;

	CollisionTileNum = 0;

	for (auto& _Tile_Col : _Tile_Collision_List)
	{
		vec TileCenter = { _Tile_Col.first + x,_Tile_Col.second + y };
		if (!math::RectInPoint(CameraRange, TileCenter - cp))continue;
		++CollisionTileNum;

		for (auto& rhs_obj : rhs_list)
		{
			if (!rhs_obj->bCollision)continue;
			vec collision_pos = rhs_obj->make_center();
			if(!math::RectInPoint(CameraRange, collision_pos - cp))continue;

			auto rhs_figure = rhs_obj->_figure_type;

			std::optional<vec> bCollision{ std::nullopt };

			RECT lhs_rect = { _Tile_Col.first,_Tile_Col.second , 
			_Tile_Col.first + game::TileWorldX, _Tile_Col.second + game::TileWorldY };

			bCollision = math::rectVSrect(lhs_rect, rhs_obj->make_rect());

			if (bCollision.has_value())
			{
				// 우항 오브젝트 밀어버리기
				if (rhs_obj->bSlide)
				{
					auto _ptr = rhs_obj->get_owner().lock();
					if (!_ptr)return;

					_ptr->_transform->_location += *bCollision;
					_ptr->HitTile(lhs_rect);
				}
			}
		}
	}
};



void collision_mgr::Insert_CollisionTile(std::pair<int, int> _location)
{
	auto iter = std::find_if(std::begin(_Tile_Collision_List), std::end(_Tile_Collision_List), [_location](auto& _Tile)
		{return _location.first == _Tile.first && _location.second == _Tile.second; });

	// 현재 마우스 월드 좌표에 콜리전이 없을때만 인서트
	if (iter == std::end(_Tile_Collision_List)) {

		_Tile_Collision_List.push_back(std::move(_location));
	}
}
void collision_mgr::Erase_CollisionTile(std::pair<int, int> _location)
{
	std::erase_if(_Tile_Collision_List, [_location](auto& _Tile)
		{return _location.first == _Tile.first && _location.second == _Tile.second; });
}
void collision_mgr::save_collision(std::wstring filename = StageFileName)
{
	size_t Tile_Num = _Tile_Collision_List.size();
	auto FileName = std::to_wstring(math::Rand<uint32_t>({ 2020,7070 }));

	std::ofstream ofs(DefaultMapCollisionPath+ FileName);

	if (!ofs.is_open())
	{
		MessageBox(game::hWnd, L"맵 충돌 데이터를 저장하는데 실패했습니다.", L"Collision Save Fail !!", MB_OK);
		throw std::exception("Collision  Save Fail !!");
	}

	ofs << Tile_Num;

	for (auto& _Tile : _Tile_Collision_List)
	{
		ofs << '|'<<_Tile.first << ',' << _Tile.second << '|';
	}


	std::wstringstream	wss;
	wss << L"맵 충돌 데이터를 저장했습니다. \n " << L" 파일 이름 : " << FileName;
	MessageBox(game::hWnd,  wss.str().c_str(), L"Collision Data Save", MB_OK);

}
void collision_mgr::load_collision(std::wstring filename= StageFileName)
{
	collision_tile_clear();

	std::ifstream ifs(DefaultMapCollisionPath + filename);

	if (!ifs.is_open())
	{
		MessageBox(game::hWnd, L"맵 충돌 데이터를 읽어오지 못했습니다.", L"Collision Load Fail !!", MB_OK);
		throw std::exception("Collision Data Load Fail !!");
	}

	size_t Collision_Num;
	ifs >> Collision_Num;

	_Tile_Collision_List.reserve(Collision_Num);

	for (int i = 0; i < Collision_Num; ++i)
	{
		int x{},y{};
		char temp;
		ifs >> temp;
		ifs >> x;
		ifs >> temp;
		ifs >> y;
		ifs >> temp;

		_Tile_Collision_List.emplace_back(x,y);
	}

	std::wstringstream wss;
	wss << L"맵 충돌 데이터를 로딩했습니다. \n " << DefaultMapCollisionPath << filename;
	MessageBox(game::hWnd, wss.str().c_str(), L"Collision Data Load", MB_OK);
};

std::weak_ptr< collision_component>
collision_mgr::insert(std::weak_ptr<class object> _owner, collision_tag _tag,
	figure_type _type)
{
	std::shared_ptr<collision_component> _collision = std::make_shared<collision_component>();

	_collision->_figure_type = _type;
	_collision->set_owner(_owner);
	_collision->_Tag = _tag;

	_collision_map[_tag].push_back(_collision);

	return _collision;
};

void collision_mgr::render(HDC hdc, std::pair<float, float> size_factor)
{
	vec cpos = object_mgr::instance().camera_pos;

	if (bDebug)
	{
		auto DBG = Debuger(hdc, RGB(255, 104, 162), RGB(255, 104, 162));

		for (auto& ct : _Tile_Collision_List)
		{

			Rectangle(hdc, ct.first - cpos.x, ct.second - cpos.y, ct.first + game::TileWorldX - cpos.x, ct.second + game::TileWorldY - cpos.y);
		}
	};

	HitEffectImg = Bmp_mgr::instance().Find_Image_SP(L"HITEFFECT");
	HDC srcDC = HitEffectImg->Get_MemDC();

	if (!HitEffectImg)return;

	{
		for (auto iter = std::begin(CollisionHitEffectList);
			iter != std::end(CollisionHitEffectList);)
		{
			auto& [v, col, row, delta] = *iter;

			delta -= DeltaTime;
			if (delta < 0)
			{
				delta = CollisionHitEffectDelta;
				++col;
			}
			if (col >= 5)
			{
				iter = CollisionHitEffectList.erase(iter);
			}
			else
			{
				++iter;
			}

			vec w = v - cpos;
			
			float wx{ 100 }, wy{ 100 };
			GdiTransparentBlt(hdc, w.x-(wx/2), w.y-(wy/2),
				wx, wy, srcDC, col * 230, row * 230, 230, 230, RGB(255, 250, 255));
		}
	}

	if (!bDebug)return;

	RECT c_rect = game::instance().client_rect;
	

	int render_object_count = 0;
	int comp_count{ 0 };

	for (auto& [tag, collisions] : _collision_map)
	{
		for (auto& collision : collisions)
		{
			comp_count++;

			if (!collision)continue;
			if (!collision->bRender) continue;;
			auto _owner = collision->get_owner().lock();
			if (!_owner)
			{
				collision->bDie = true;
				continue;
			}
			if (!bDebug)continue;

				vec _loc = _owner->_transform->_location;
				vec _size = collision->_size;
				_loc -= collision->correction;

				RECT _rt;

				RECT _rhs{ _loc.x - _size.x
						- cpos.x, _loc.y - _size.y - cpos.y, _loc.x + _size.x - cpos.x, _loc.y + _size.y
						- cpos.y };

				if (!IntersectRect(&_rt, &c_rect, &_rhs))continue;

				auto DBG = Debuger(hdc, [&]() {
					if (collision->_figure_type == ERect)
					{
						Rectangle(hdc, _loc.x - _size.x
							- cpos.x, _loc.y - _size.y - cpos.y, _loc.x + _size.x - cpos.x, _loc.y + _size.y
							- cpos.y);

						++render_object_count;
					}
					else if (collision->_figure_type == ECircle)
					{
						Ellipse(hdc, _loc.x - _size.x
							- cpos.x, _loc.y - _size.y - cpos.y, _loc.x + _size.x - cpos.x, _loc.y + _size.y
							- cpos.y);

						++render_object_count;
					}
					});
			}
		}
		auto DBG = Debuger(hdc, [&]() {std::wstringstream wss;
		wss << L"렌더링 되는 충돌체 : " << render_object_count << std::endl;
		RECT _rt{ 1200,300,1600,400 };
		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
		std::wstring wstr = L" 충돌체의 개수 :" + std::to_wstring(comp_count);
		TextOut(hdc, 1400, 100, wstr.c_str(), wstr.size());
			});

		if (bDebug)
		{
			{
				std::wstringstream wss;
				wss << L"충돌검사 범위 안 타일 개수 : " << CollisionTileNum << std::endl;
				Font(game::hDC, RGB(111, 111, 111), 0, 400, 30, wss.str());
			}
			
			std::wstringstream wss;

			wss << L"충돌검사 범위 안 오브젝트 개수 : " << CollisionObjNum << std::endl;
			Font(game::hDC, RGB(111, 111, 111), 0, 200, 30, wss.str());

		}
};
void collision_mgr::update()
{
	CollisionRangeX = (game::client_rect.right - game::client_rect.left) / 2;
	CollisionRangeY = (game::client_rect.bottom - game::client_rect.top) / 2;

	CollisionObjNum = 0;

	collision(EMonster, EMonster);
	collision(EMonster, EPlayer);
	collision(EPlayerAttack, EMonster);
	collision(EFireDragon, EMonster);
	collision(EMonsterAttack, EPlayer);
	collision(EShield, EMonster);
	collision(EShield,EMonsterAttack);



	collision_tile(EPlayer);
	collision_tile(EMonster);
	collision_tile(EFireDragon);
	collision_tile(EPlayerAttack);
	collision_tile(EMonsterAttack);

	check_erase();
}
void collision_mgr::release()
{
	_collision_map.clear();

	collision_tile_clear();
}
bool collision_mgr::IsHitEffectMappingTag(collision_tag lhs, collision_tag rhs)
{
	if (lhs == collision_tag::EMonster && rhs == collision_tag::EMonster)return false;

	return true;
}
bool collision_mgr::IsObjectSlideMappingTag(collision_tag lhs, collision_tag rhs)
{
	if (lhs == collision_tag::EMonster && rhs == collision_tag::EMonster)
	{

		int i = 0;




		return true;
	}

	return false;
}
void collision_mgr::check_erase()&
{
	for (auto& [tag, collision_list] : _collision_map)
	{
		collision_list.erase(

			std::remove_if(std::begin(collision_list), std::end(collision_list),
				[](std::shared_ptr<collision_component>& collision_comp) {
					if (!collision_comp)return true;
					return collision_comp->bDie;
				}),

			std::end(collision_list));
	}
};

void collision_mgr::collision(collision_tag lhs, collision_tag rhs)
{
	auto& lhs_list = _collision_map[lhs];
	auto& rhs_list= _collision_map[rhs];

	RECT CameraRange = game::client_rect;
	CameraRange.left -= CollisionRangeX;
	CameraRange.top -= CollisionRangeY;
	CameraRange.right += CollisionRangeX;
	CameraRange.bottom += CollisionRangeY;

	vec cp = object_mgr::instance().camera_pos;

	int x = game::TileWorldX / 2;
	int y = game::TileWorldY / 2;

	for (auto&   lhs_obj : lhs_list)
	{
		if (!lhs_obj->bCollision)continue;
		if (!math::RectInPoint(CameraRange, lhs_obj->make_center()- cp))continue;

		for (auto& rhs_obj : rhs_list)
		{
			if (!rhs_obj->bCollision)continue;
			if (&lhs_obj == &rhs_obj)continue;
			if (!math::RectInPoint(CameraRange, rhs_obj->make_center() - cp))continue;

			++CollisionObjNum;
			// 도형 정보에 따라 충돌 다르게 수행
			auto lhs_figure = lhs_obj->_figure_type;
			auto rhs_figure = rhs_obj->_figure_type;

			std::optional<vec> bCollision{ std::nullopt };

			if (lhs_figure == ERect && rhs_figure == ERect)
			{
				bCollision= math::rectVSrect(lhs_obj->make_rect(), rhs_obj->make_rect());
			}
			else if ( lhs_figure == ECircle && rhs_figure == ECircle)
			{
				bCollision = math::circleVScircle(lhs_obj->make_circle(), rhs_obj->make_circle());
			}
			else if (lhs_figure == ERect && rhs_figure == ECircle)
			{
				bCollision = math::rectVScircle(lhs_obj->make_rect(), rhs_obj->make_circle());
			}
			else if (lhs_figure == ECircle&& rhs_figure == ERect)
			{
				bCollision = math::circleVSrect(lhs_obj->make_circle(), rhs_obj->make_rect());
			}

			if (bCollision.has_value())
			{
				lhs_obj->Hit(rhs_obj->get_owner());
				rhs_obj->Hit(lhs_obj->get_owner());

				auto _ptr = rhs_obj->get_owner().lock();

				if (!_ptr)
				{
					rhs_obj->bDie = true;
					return;
				}
				
				// 우항 오브젝트 밀어버리기
				if (lhs_obj->bPush    )
				{
					_ptr->_transform->_location += (*bCollision).get_normalize()
						* lhs_obj->PushForce;
				}
				if (lhs_obj->bObjectSlide || IsObjectSlideMappingTag(lhs_obj->_Tag, rhs_obj->_Tag))
				{
					_ptr->_transform->_location += *bCollision;
				}
				if (lhs_obj->bHitEffect)
				{
					if (!_ptr->_transform)continue;
					if (false == IsHitEffectMappingTag(lhs_obj->_Tag, rhs_obj->_Tag))return;

					auto sp_owner = lhs_obj->get_owner().lock();
					if (!sp_owner)
					{
						lhs_obj->bDie = true;
						continue;
					}
					
					float dt = CollisionHitEffectDelta;
					int Row = math::Rand<int>({ 0,3 });
					int Col = 0;

					vec r = _ptr->_transform->_location + math::RandVec() * math::Rand<int>({ -30,30});

					CollisionHitEffectList.push_back({r,Col,Row,dt});
				}
			}
		}
	}
}
