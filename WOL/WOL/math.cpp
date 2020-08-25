#include "pch.h"
#include "math.h"


// ��ü �����κ��� ���� ����

vec math::dir_from_angle(const float degree)
{
	float _radian = degree_to_radian(degree);

	return  { std::cosf(_radian), std::sinf(_radian) };
}

// �̹� �����ϴ� �������κ��� ���� ���ҷ��� �ޱ۸�ŭ ���� ȸ�����Ѽ� ����
vec math::rotation_dir_to_add_angle(const vec dir,
	const float degree)
{
	// ���� - �ν�
	// ���� + �ڽ�
	const auto _radian = math::degree_to_radian(degree);
	const float b_cos = std::cosf(_radian);
	const float b_sin = std::sinf(_radian);

	return { dir.x * b_cos - dir.y * b_sin,
		dir.y * b_cos + dir.x * b_sin };
}


std::optional<vec>   math::rectVScircle(RECT _rect, circle _circle)
{
	// left,top , right,top , left,bottom , right,bottom �� ������

	float _radius_pow2 = pow(_circle._radius, 2);

	
	vec v0 = { _rect.left,_rect.top };
	vec point_to_circle = _circle._pos - v0;
	float length = point_to_circle.length_2();
	vec _rect_center = { (_rect.right + _rect.left) / 2 ,(_rect.bottom + _rect.top) / 2 };


	if (length < _radius_pow2)
	{
		return { _circle._pos - _rect_center };
	}

	v0 = { _rect.right,_rect.top };
	point_to_circle = _circle._pos - v0;
	length = point_to_circle.length_2();

	if (length < _radius_pow2)
	{
		return { _circle._pos - _rect_center };
	}

	v0 = { _rect.left,_rect.bottom };
	point_to_circle = _circle._pos - v0;
	length = point_to_circle.length_2();

	if (length < _radius_pow2)
	{
		return { _circle._pos - _rect_center };
	}

	v0 = { _rect.right,_rect.bottom };
	point_to_circle = _circle._pos - v0;
	length = point_to_circle.length_2();
	if (length < _radius_pow2)
	{
		return { _circle._pos - _rect_center };
	}

	float r = _circle._radius;
	RECT c = { _rect.left - r,_rect.top - r,_rect.right + r ,_rect.bottom + r };
	vec p = _circle._pos;

	if (p.x >= c.left && p.x <= c.right && p.y <= c.bottom && p.y >= c.top)
	{
		RECT _rt;
		RECT lhs = _rect;
		RECT rhs = {p.x - r,p.y - r , p.x +r,p.y +r};

		if (IntersectRect(&_rt, &lhs, &rhs))
		{
			float h = _rt.bottom - _rt.top;
			float w = _rt.right - _rt.left;

			if (h < w)
			{
				float dir = rhs.bottom - lhs.bottom;
				dir = dir / abs(dir);

				return vec{ 0,h * dir };
			}
			if (w < h)
			{
				float dir = rhs.right - lhs.right;
				dir = dir / abs(dir);

				return vec{ w * dir,0 };
			}
		}
	}

	return std::nullopt;
};

std::optional<vec> math::circleVSrect(circle _circle, RECT _rect)
{
	auto dir = rectVScircle(_rect, _circle);

	if (dir.has_value())
	{
		return (*dir) * -1;
	}

	return dir;
}
std::optional<vec> math::circleVScircle(circle lhs, circle rhs)
{
	float radius = (lhs._radius + rhs._radius);

	vec v = rhs._pos - lhs._pos;

	if (v.length() < radius)
	{
		return { v };
	}

	return std::nullopt;
}
std::optional<vec> math::rectVSrect(RECT lhs, RECT rhs)
{
	RECT _rt;

	if (IntersectRect(&_rt ,&lhs, &rhs ))
	{
		float h = _rt.bottom - _rt.top;
		float w = _rt.right - _rt.left;

		if (h < w)
		{
			float dir = rhs.bottom - lhs.bottom;
			dir = dir / abs(dir);

			return vec{ 0,h*dir };
		}
		if (w < h)
		{
			float dir = rhs.right-lhs.right;
			dir = dir / abs(dir);

			return vec{ w*dir,0 };
		}
	}

	return std::nullopt;
};


vec::vec(float _x, float _y) : x{ _x }, y{ _y }
{
}

vec::vec(const float degree) : x{ std::cosf(math::degree_to_radian(degree)) },
y{ std::sinf(math::degree_to_radian(degree)) }
{

}

vec& vec::operator-=(const vec& _rhs)
{
	if (this == &_rhs)
		return *this;

	x -= _rhs.x;
	y -= _rhs.y;
	return *this;
};


vec& vec::operator+=(const vec& _rhs)
{
	if (this == &_rhs)
		return *this;

	x += _rhs.x;
	y += _rhs.y;
	return *this;
}

vec& vec::operator*=(const float _scala)
{
	x *= _scala;
	y *= _scala;

	return *this;
}

vec& vec::operator/=(const vec& _rhs)
{
	if (this == &_rhs)
		return *this;

	x /= _rhs.x;
	y /= _rhs.y;
	return *this;
}

vec& vec::operator-=(const float _scala)
{
	x -= _scala;
	y -= _scala;
	return *this;
}

vec& vec::operator+=(const float _scala)
{
	x += _scala;
	y += _scala;
	return *this;
}


vec& vec::operator/=(const float _scala)
{
	x /= _scala;
	y /= _scala;
	return *this;
}

vec vec::operator+(const vec& _rhs) const
{
	return vec{ x + _rhs.x , y + _rhs.y };
}

vec vec::operator-(const vec& _rhs) const
{
	return vec{ x - _rhs.x , y - _rhs.y };
}

float  vec::operator*(const vec& _rhs) const
{
	return dot(_rhs);
}


float vec::dot(const vec& _rhs) const
{
	return x * _rhs.x + y * _rhs.y;
}

vec vec::get_normalize() const
{
	float l = length();
	return { x / l , y / l };
}

float vec::length() const
{
	return std::sqrtf(length_2());
}

float vec::length_2() const
{
	return (x * x + y * y);
}

bool vec::operator==(const vec& _rhs) const
{
	return EQUAL(x, _rhs.x) && EQUAL(y, _rhs.y);
}

bool vec::operator!=(const vec& _rhs) const
{
	return !(*this == _rhs);
}

bool vec::operator<(const vec& _rhs) const
{
	return length_2() < _rhs.length_2();
}

bool vec::operator<=(const vec& _rhs) const
{
	float l, m;
	l = this->length_2();
	m = _rhs.length_2();
	return l < m&& EQUAL(l, m);
}

bool vec::operator>(const vec& _rhs) const
{
	return length_2() > _rhs.length_2();
}

bool vec::operator>=(const vec& _rhs) const
{
	float l, m;
	l = this->length_2();
	m = _rhs.length_2();
	return l > m && EQUAL(l, m);
}

