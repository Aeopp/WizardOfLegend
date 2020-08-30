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


void collision_mgr::collision_tile_clear()
{
	if(!_Tile_Collision_List.empty())
	_Tile_Collision_List.clear();
};

void collision_mgr::collision_tile(collision_tag rhs)
{
	auto& rhs_list = _collision_map[rhs];

	for (auto& _Tile_Col : _Tile_Collision_List)
	{
		for (auto& rhs_obj : rhs_list)
		{
			if (!rhs_obj->bCollision)continue;

			auto rhs_figure = rhs_obj->_figure_type;

			std::optional<vec> bCollision{ std::nullopt };

			RECT lhs_rect = { _Tile_Col.first,_Tile_Col.second , _Tile_Col.first + game::TileWorldX, _Tile_Col.second + game::TileWorldY };
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
}
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

	int I = math::Rand<int>({ 1, 100000000 });

	std::ofstream ofs(DefaultMapCollisionPath+ std::to_wstring(I));
	ofs << Tile_Num;

	for (auto& _Tile : _Tile_Collision_List)
	{
		ofs << '|'<<_Tile.first << ',' << _Tile.second << '|';
	}
}
void collision_mgr::load_collision(std::wstring filename= StageFileName)
{
	collision_tile_clear();

	std::ifstream ifs(DefaultMapCollisionPath + filename);

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
};

std::weak_ptr< collision_component>
collision_mgr::insert(std::weak_ptr<class object> _owner, collision_tag _tag,
	figure_type _type)
{
	std::shared_ptr<collision_component> _collision = std::make_shared<collision_component>();

	_collision->_figure_type = _type;
	_collision->set_owner(_owner);
	_collision_map[_tag].push_back(_collision);

	return _collision;
};

void collision_mgr::render(HDC hdc, std::pair<float, float> size_factor)
{
	if (!bRender)return;
	if (!bDebug)return;

	RECT c_rect = game::instance().client_rect;

	vec cpos = object_mgr::instance().camera_pos;

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
				vec _loc = _owner->_transform->_location;
				vec _size = collision->_size;

				RECT _rt;

				RECT _rhs{ _loc.x - _size.x
						- cpos.x, _loc.y - _size.y - cpos.y, _loc.x + _size.x - cpos.x, _loc.y + _size.y
						- cpos.y };

				if (!IntersectRect(&_rt, &c_rect, &_rhs))continue;

				Debuger(hdc, [&]() {
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
		Debuger(hdc, [&]() {std::wstringstream wss;
		wss << L"렌더링 되는 충돌체 : " << render_object_count << std::endl;
		RECT _rt{ 1200,300,1600,400 };
		DrawText(hdc, wss.str().c_str(), wss.str().size(), &_rt, DT_LEFT);
		std::wstring wstr = L" 충돌체의 개수 :" + std::to_wstring(comp_count);
		TextOut(hdc, 1400, 100, wstr.c_str(), wstr.size());
			});

		if (bDebug)
		{
			Debuger(hdc, RGB(106, 101, 88), RGB(255,255,255));

			for (auto& ct : _Tile_Collision_List)
			{

				Rectangle(hdc, ct.first- cpos.x, ct.second- cpos.y, ct.first + game::TileWorldX- cpos.x, ct.second + game::TileWorldY- cpos.y);
			}
		}
};

void collision_mgr::update()
{
	collision(EMonster, EPlayer);
	collision(EPlayerAttack, EMonster);
	collision(EMonsterAttack, EPlayer);

	collision_tile(EPlayer);
	collision_tile(EMonster);
	collision_tile(EPlayerAttack);
	check_erase();
}
void collision_mgr::release()
{
	_collision_map.clear();

	collision_tile_clear();
}
void collision_mgr::check_erase()&
{
	for (auto& [tag, collision_list] : _collision_map)
	{
		collision_list.erase(

			std::remove_if(std::begin(collision_list), std::end(collision_list),
				[](auto& collision_comp) {if (!collision_comp)return true; return collision_comp->bDie; }),

			std::end(collision_list));
	}
};

void collision_mgr::collision(collision_tag lhs, collision_tag rhs)
{
	auto& lhs_list = _collision_map[lhs];
	auto& rhs_list= _collision_map[rhs];

	for (auto&   lhs_obj : lhs_list)
	{
		if (!lhs_obj->bCollision)continue;

		for (auto& rhs_obj : rhs_list)
		{
			if (!rhs_obj->bCollision)continue;

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
				if (!_ptr)return;
				
				// 우항 오브젝트 밀어버리기
				if (rhs_obj->bSlide)
				{
					_ptr->_transform->_location += *bCollision;
				}
				if (lhs_obj->bPush)
				{
					_ptr->_transform->_location += (*bCollision).get_normalize()
					* lhs_obj->PushForce;
				}
			}
		}
	}
}
