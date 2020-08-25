#pragma once
#include <numeric>
#include <numbers>
#include <cmath>
#include <type_traits>
#include <optional>

struct vec
{
	float x{}; float y{};
	// ========================================
	vec() = default;
	~vec()noexcept = default;
	vec(const vec&) = default;
	vec(vec&&)noexcept = default;
	vec& operator=(vec&&)noexcept = default;
	vec& operator=(const vec&) = default;
	template<typename _Ty>
	vec(_Ty _x, _Ty _y) : x{ (float)_x }, y{ (float)_y }{};
	vec(float _x, float _y);
	vec(const float degree);
	// ========================================
	vec& operator-=(const vec& _rhs);
	vec& operator+=(const vec& _rhs);
	vec& operator/=(const vec& _rhs);
	
	vec& operator-=(const float _rhs);
	vec& operator+=(const float _rhs);
	vec& operator*=(const float _rhs);
	vec& operator/=(const float _rhs);

	vec operator+(const vec& _rhs)const;
	vec operator-(const vec& _rhs)const;
	float operator*(const vec& _rhs)const;

	float dot(const vec& _rhs)const;

	vec get_normalize()const;
	float length()const;
	float length_2()const;

	bool operator==(const vec& _rhs)const;
	bool operator!=(const vec& _rhs)const;
	bool operator<(const vec& _rhs)const;
	bool operator<=(const vec& _rhs)const;
	bool operator>(const vec& _rhs)const;
	bool operator>=(const vec& _rhs)const;
};
struct circle
{

	vec _pos;
	float _radius{ 0.0f };

	circle() = default;
	~circle() noexcept = default;
	circle(const circle&) = default;
	circle(circle&&) noexcept= default;
	circle& operator=(circle&&)noexcept = default;
	circle& operator=(const circle&) = default;

	circle(vec _pos, float _radius) : 
		_pos{ std::move(_pos) }, 
		_radius{ _radius }{};
};

class math
{
public:
	static inline constexpr float degree_to_radian(const float degree)
	{
		constexpr float  factor = (float)std::numbers::pi / (float)180.0f;

		return factor * degree;
	};
	static inline constexpr float radian_to_degree(const float radian)
	{
		constexpr float factor = 180.0f / (float)std::numbers::pi;

		return factor * radian;
	};
	// ��ü �����κ��� ���� ����
	static vec dir_from_angle(const float degree);
	// �̹� �����ϴ� �������κ��� ���� ���ҷ��� �ޱ۸�ŭ ���� ȸ�����Ѽ� ����
	static vec rotation_dir_to_add_angle(const vec dir,const float degree);

	
	// �簢���� ���� �浹 �簢���� �װ��� �������� ���ȿ� �ִ��� �˻��մϴ�.
	// ���� ���õ��ִٸ� �浹 �ƴ϶�� �浹�� �ƴ�.
	// ��ȯ�Ǵ� ������ ������ �浹�� �������� ���� ��Ŭ�� �߽����� �����̰�
	// ������ ���̴� ������ ���� ��ŭ�̴�. (��ֶ����� ���� ����)
	static std::optional<vec> rectVScircle(RECT _rect, circle _circle);
	static std::optional<vec> circleVSrect(circle _circle,RECT _rect);
	// ��ȯ�Ǵ� ���ʹ� ���׿��� �������� ���ϴ� ���� ���̴� ������ ��ŭ( ��ֶ����� ���� ����)
	static std::optional<vec> circleVScircle(circle lhs, circle rhs);
	// ��ȯ�Ǵ� ���ʹ� ���׿��� �������� ���ϴ� �������� ���������� ������ �밢���� �ƴϰ� x�� y�� �����ϴ� ����
	// ���� ���̴� ������ ��ŭ( ��ֶ����� ���� ����)
	// ��Ʈ�� �浹������ ���������� �о��ִ°� �ڿ������� ������.
	static std::optional<vec> rectVSrect(RECT lhs, RECT rhs);
};

static  bool EQUAL(const float lhs, const float rhs)
{
	if (std::abs(lhs - rhs) < std::numeric_limits<float>::epsilon())
	{
		return true;
	}
	// else 
	return false;
}
