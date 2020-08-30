#pragma once

#include "pch.h"
#include <iostream>

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
	static vec make_vec_from_dir(const float degree);
	// ========================================
	vec& operator-=(const vec& _rhs);
	vec& operator+=(const vec& _rhs);
	vec& operator/=(const vec& _rhs);

	vec& operator-=(const float _rhs);
	vec& operator+=(const float _rhs);
	vec& operator*=(const float _rhs);
	vec& operator/=(const float _rhs);

	vec operator*(const float _scala)const;
	vec operator+(const vec& _rhs)const;
	vec operator-(const vec& _rhs)const;

	template<typename T>
	std::pair<T, T> Pair()
	{
		return std::pair<T, T>{x, y};
	}

	template<typename T>
	vec(std::pair<T, T> Pair) : x{ (float)Pair.first }, y{ (float)Pair.second }{};

	float operator*(const vec& _rhs)const;

	float dot(const vec& _rhs)const;


	// 0~180 0~-180 각도를 360도 기준 각도로 변환 

	vec get_normalize()const;
	float length()const;
	float length_2()const;

	bool operator==(const vec& _rhs)const;
	bool operator!=(const vec& _rhs)const;
	bool operator<(const vec& _rhs)const;
	bool operator<=(const vec& _rhs)const;
	bool operator>(const vec& _rhs)const;
	bool operator>=(const vec& _rhs)const;	

	static vec unit_diagonal_vec();



	
};
std::wostream& operator<<(std::wostream& os, vec _lhs);


struct circle
{
	vec _pos;
	float _radius{ 0.0f };

	circle() = default;
	~circle() noexcept = default;
	circle(const circle&) = default;
	circle(circle&&) noexcept = default;
	circle& operator=(circle&&)noexcept = default;
	circle& operator=(const circle&) = default;

	circle(vec _pos, float _radius) :
		_pos{ std::move(_pos) },
		_radius{ _radius }{};
};

class math
{
public:
	enum class EDir : uint8_t
	{
		left,
		right,
		up,
		down,
	};
	static inline float PI = std::numbers::pi;

	static inline float cos45 = cosf(PI / 4);

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
	// 방향 리턴 
	static EDir checkDir(vec dir);
	// 전체 각으로부터 방향 리턴
	static vec dir_from_angle(const float degree);
	// 이미 존재하는 방향으로부터 새로 더할려는 앵글만큼 방향 회전시켜서 리턴
	static vec rotation_dir_to_add_angle(const vec dir, const float degree);


	// 사각형과 원의 충돌 사각형의 네개의 꼭짓점이 원안에 있는지 검사합니다.
	// 값이 세팅되있다면 충돌 아니라면 충돌이 아님.
	// 반환되는 벡터의 방향은 충돌한 꼭짓점이 시점 서클의 중심점이 종점이고
	// 벡터의 길이는 교차한 길이 만큼이다. (노멀라이즈 하지 않음)
	static std::optional<vec> rectVScircle(RECT _rect, circle _circle);
	static std::optional<vec> circleVSrect(circle _circle, RECT _rect);
	// 반환되는 벡터는 좌항에서 우항으로 향하는 벡터 길이는 교차한 만큼( 노멀라이즈 하지 않음)
	static std::optional<vec> circleVScircle(circle lhs, circle rhs);
	// 반환되는 벡터는 좌항에서 우항으로 향하는 벡터지만 예외적으로 방향은 대각선이 아니고 x나 y와 평행하는 벡터
	// 벡터 길이는 교차한 만큼( 노멀라이즈 하지 않음)
	// 렉트는 충돌면적이 작은쪽으로 밀어주는게 자연스럽기 때문에.
	static std::optional<vec> rectVSrect(RECT lhs, RECT rhs);
static 	float Angle360conversion(float degree);

	static bool RectInPoint(RECT rt, vec p);
public:
	static float my_lerp(float start, float goal, float goal_time,float dt);

	static inline std::random_device rd;

	template<typename Ty>
	static Ty Rand_Normal(std::pair<Ty, Ty> range);

	template<typename Ty>
	static Ty Rand(std::pair<Ty, Ty> range);
};

static  bool EQUAL(const float lhs, const float rhs)
{
	if (std::abs(lhs - rhs) < std::numeric_limits<float>::epsilon())
	{
		return true;
	}
	// else 
	return false;
};

template<typename Ty>
Ty math::Rand_Normal(std::pair<Ty, Ty> range)
{
	static std::mt19937 gen(rd());

	if constexpr (std::is_floating_point_v<Ty>)
	{
		std::uniform_real_distribution<Ty> dis(range.first, range.second);
		return dis(gen);
	}
	else  {
		std::uniform_int_distribution<Ty> dis(range.first, range.second);
		return dis(gen);
	}

	
}

template<typename Ty>
Ty math::Rand(std::pair<Ty, Ty> range)
{
	static std::mt19937 gen(rd());

	if constexpr (std::is_floating_point_v<Ty>)
	{
		std::uniform_real<Ty> dis(range.first, range.second);
		return dis(gen);
	}
	else {
		std::uniform_int<Ty> dis(range.first, range.second);
		return dis(gen);
	}
}
	