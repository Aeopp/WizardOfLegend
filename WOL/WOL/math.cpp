#include "pch.h"
#include "math.h"
#include "game.h"


// 전체 각으로부터 방향 리턴



// 화면 좌표로 그대로 쓸것이라면 뒤집어야함에 유의할것.
math::EDir math::checkDir(vec dir)
{
	dir = dir.get_normalize();

	// 이 값보다 크면 45도 이내에 있음
	
	if (dir.dot(vec{ +1,0 }) > cos45)
	{
		return EDir::right;
	}
	else if (dir.dot(vec{ -1,0 }) > cos45)
	{
		return EDir::left;
	}
	else if (dir.dot(vec{ 0,+1 }) > cos45)
	{
		return EDir::up;
	}
	else if (dir.dot(vec{ 0,-1 }) > cos45)
	{
		return EDir::down;
		
	}
	return EDir();
}

vec math::dir_from_angle(const float degree)
{
	float _radian = degree_to_radian(degree);

	return  { std::cosf(_radian), std::sinf(_radian) };
}

// 이미 존재하는 방향으로부터 새로 더할려는 앵글만큼 방향 회전시켜서 리턴
vec math::rotation_dir_to_add_angle(const vec dir,
	const float degree)
{
	// 코코 - 싸싸
	// 싸코 + 코싸
	const auto _radian = math::degree_to_radian(degree);
	const float b_cos = std::cosf(_radian);
	const float b_sin = std::sinf(-_radian);

	return { dir.x * b_cos - dir.y * b_sin,
		dir.y * b_cos + dir.x * b_sin };
}

float math::AngleFromVec(vec v)
{
	return math::Angle360conversion(
		math::radian_to_degree(atan2f(v.y, v.x)));
}


std::optional<vec>   math::rectVScircle(RECT _rect, circle _circle)
{
	float r = _circle._radius;
	vec t1 = _circle._pos;

	float left = (_rect.left - r);
	float top = (_rect.top - r);
	float bottom= (_rect.bottom + r);
	float right = (_rect.right + r);

	if ((t1.x >= left && t1.x <= right) &&
		(t1.y >= top && t1.y <= bottom))
	{
		vec _c = {  (_rect.right + _rect.left) / 2 , (_rect.bottom + _rect.top )/ 2 };
		vec dis = _circle._pos - _c;

		if (abs(dis.y) < abs(dis.x) )
		{
			float l = (_rect.right - _rect.left) / 2 + _circle._radius;

			return vec{ dis.x/abs(dis.x) *(l-abs(dis.x)), 0 };
		}
		else 
		{
			float l = (_rect.bottom - _rect.top) / 2 + _circle._radius;

			return vec{ 0,dis.y / abs(dis.y) * (l - abs(dis.y)) };
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
		return v.get_normalize() * (radius - v.length());
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
}
bool math::RectInPoint(RECT rt, vec p)
{
	if (p.x >= rt.left && p.x <= rt.right && p.y >= rt.top && p.y <= rt.bottom)
		return true;
	return false;
}
float math::my_lerp(float start, float goal, float goal_time,float dt)
{
	float f = goal - start;
	return start + ((f * dt) / goal_time);
}
vec math::RandVecNormal()
{
	return vec{ Rand_Normal<float>({ -1,1 }),
	Rand_Normal<float>({ -1,1 }) };
}
vec math::RandVec()
{
	return vec{ Rand<float>({ -1,1 }),
	Rand<float>({ -1,1 }) };
};



vec::vec(float _x, float _y) : x{ _x }, y{ _y }
{
}



vec vec::make_vec_from_dir(const float degree)
{
	return vec(std::cosf(math::degree_to_radian(degree)), std::sinf(math::degree_to_radian(degree)));
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

vec vec::operator*(const float _scala) const
{
	return vec(x*_scala,y*_scala);
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

float math::Angle360conversion(float degree)
{
	if (degree < 0)
		return degree *= -1;
	else
		return (180 + (180 - degree));
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

 vec vec::unit_diagonal_vec()
{
	return vec{ 0.7071067811865475 ,0.7071067811865475 };
}


std::wostream& operator<<(std::wostream& os, vec _lhs)
{
	os << L" X : " << _lhs.x << L" Y : " << _lhs.y << std::endl;
	return os;
}
