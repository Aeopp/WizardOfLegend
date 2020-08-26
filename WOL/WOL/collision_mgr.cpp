#include "pch.h"
#include "collision_mgr.h"
#include "object.h"
#include "object_mgr.h"
#include "game.h"
#include "Debuger.h"

std::weak_ptr< collision_component>
collision_mgr::insert(std::weak_ptr<class object> _owner, collision_tag _tag,
	figure_type _type)
{
	std::shared_ptr<collision_component> _collision = std::make_shared< collision_component>();

	_collision->_figure_type = _type;
	_collision->set_owner(_owner);
	_collision_map[_tag].push_back(_collision);

	return _collision;
};

void collision_mgr::render(HDC hdc, std::pair<float,float> size_factor)
{
	if (!bRender)return;

	RECT c_rect = game::instance().client_rect;

	vec cpos  = object_mgr::instance().camera_pos;

	int render_object_count = 0;

	for (auto& [tag, collisions] : _collision_map)
	{
		for (auto& collision : collisions)
		{
			if (!collision)continue;
			if (!collision->bRender) continue;;
			auto _owner = collision->get_owner().lock();
			if (!_owner)continue;

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
	});
}

void collision_mgr::update()
{
	collision(EMonster,EPlayer);
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

				// 우항 오브젝트 밀어버리기
				if (rhs_obj->bPush)
				{
					auto _ptr = rhs_obj->get_owner().lock();
					if (!_ptr)return;

					_ptr->_transform->_location += *bCollision;
				}
			}
		}
	}
}
