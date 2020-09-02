#pragma once
#include "actor.h"
#include <unordered_map>
#include <queue>

struct Fire
{
		int col;
		int row;
};
class FireDragon :
    public actor
{
public:
	static inline std::unordered_map<int32_t, int>LeftDirSpriteTable
	{ {9,0},{8,1},{7,2},{6,3},{7,4},
	{8,5},{9,6},{10,7},{11,8},
	{12,9},{13,10},{14,11},{15,12},{16,13},{17,14},{18,15},{19,16},{20,17},{21,18} };
	// 360�� degree ���� ������ 15���� �������� Ű ���� ��������Ʈ �ε���
	static inline std::unordered_map<int32_t, int>RightDirSpriteTable
	{ {0,9},{1,10},{2,11},{3,12},{4,13},{5,14},{6,15},{7,15} ,
	{15,0},{16,1},{17,2},{18,3},{19,4},{20,5},{21,6},{22,7},{23,8} };
public:
	void initialize()override;
	void render(HDC hdc, vec camera_pos, vec size_factor)override;
	Event update(float dt)override;
	uint32_t get_layer_id()const& override;
	std::weak_ptr<class collision_component>  _collision_component;
	void Hit(std::weak_ptr<class object> _target)override;
	void HitTile(RECT rt)override;
public:
	// ������ �߽���
	vec rotation_center{};
	// ���� ����
	vec rotation_center_dir{};
	// ���� �ٶ󺸴� ����
	vec rot_dir;

	// �������� 90�� ȸ���� ����
	vec Cross;

	float UpdownDistance{};

	// ���� ȸ���ϴ� �ӵ� /s
	float amplitudeSpeed = 45.f;
	float amplitude = 70.f;
	float ParticleBetWeen;
	float ParticlePaintScale{};

	vec particlePaintSize{};
	int CurrentParticleIndex = 0;
	float DefaultParticleDelta = 0.1f;
	float ParticleDelta = DefaultParticleDelta;
	int ParticleNum = 8;
	std::deque<Fire> ParticleRender{};

	std::weak_ptr<class Bmp> FireParticle{};

	float CurrentAngle = 0.f;
	
	int Updown = 1;
	float factor = 1.f;
	float Tick{};
	float Duration{};
	float Scale{};
};

