#pragma once
#include "object.h"
#include "Shadow.h"
#include "SOIL.h"
#include "ICE_Blast.h"
#include "Freezing_Interface.h"
#include "Burning_Interface.h"

struct PatternInfo
{
    float AnimDelta;
    float Duration;
    std::function<void()> Skill;
};
class BOSS : public object , public Freezing_Interface , public Burning_Interface
{
public:
    enum class EState : uint8_t
    {
        IDLE,
        Taunt,
        ATTACK,
        HIT,
        DIE,
    };
    enum  EPattern 
    {
       // �ڽ� 3��
       BOXATTACK,
       // �ڽ� 6��
       ROTBOXATTACK,
       // �ε��ڼ����� �÷��̾� ���� ��� ����
       PILLARMULTIPLEATTACK,
       // ���� ���� ���� �ֺ� ��� ��ȯ ����
       PILLARSPIRALATTACK,
       // �ڽ��� ������ ��� ��ȯ�ϴ� ����
       BOXDIRECTPILLARATTACK,
        None  ,
    };
    std::vector<int> PatternTable
    { 0,1,4,3,2,4,3,3,1,2,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4 };
    std::unordered_map<int, std::function<void()>> PatternMap;

    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void SetUp(std::weak_ptr<class object> AttackTarget, vec Location);
    virtual ~BOSS()noexcept;

    vec RenderCorrection{ 0,0 };

    // ������ ������ ���� �����ӵ��� ���ӵ� ()
    static inline const float JumpAcceleration = 500.f;
    // ���� ���ݽÿ� ������ ���Ŀ� �÷��̾ ���� �����ϴ� �ӵ�
    static inline const float JumpInitTrackSpeed = 400;
    float CurrentJumpAcceleration = 0;
    void JumpStart();
    void JumpTracking();
    void JumpEnd();
    void JumpEndAnimPlay();

    //left right ��������Ʈ
    std::pair< std::shared_ptr<class Bmp>, std::shared_ptr<class Bmp> > sp_Bmps;
    std::shared_ptr<class Bmp> sp_AttackBmp;
    std::shared_ptr<class Bmp> sp_SOILBmp;

    std::shared_ptr<class Bmp >CurrentBmp; 

    std::weak_ptr<class collision_component> wp_Attackcollision;
    std::weak_ptr<class collision_component> wp_collision;
    std::weak_ptr<class object> wp_AttackTarget;
    std::shared_ptr<class BossInfo> sp_MyInfo{};
    std::weak_ptr<class UIBossHPBar> wp_UIBossHPBar{};
    std::weak_ptr<class UIBossName> wp_UIBossName{};

    Shadow _Shadow;
    EPattern CurrentPattern;
	
    int PaintSizeX;
    int PaintSizeY;
    float ScaleX;
    float ScaleY;
    int CurrentRowIdx = 0;
    int CurrentColIdx = 0;
    float Speed;
    static inline float AnimDelta = 0.15f;
    vec AttackDir{};
    vec MoveMark{};

    float CurrentAnimDelta = AnimDelta;
    float AttackStartDistance;
    float StateDuration;
    float CurrentHitCoolTime = 0.1f;
    static inline float DefaultHitCoolTime = 0.3f;

    std::function<void()> CurrentSKILL;
    // Left Right ��������Ʈ ���̺�
    std::vector<uint32_t> CurrentAnimColNumTalble{ 1,9,2,8,2,1 };
    // Attack ��������Ʈ ���̺�
    std::vector<uint32_t> CurrentAttackAnimColNumTable{ 4,4,4,4 };
    bool bAttackAnimSprite = false;
    float TargetDistance = 0; 
    void AttackAnimOn();
    void AttackAnimEnd();
    int CurrentAnimColMax = 0;

    static inline const  std::pair<float, float > DefaultShadowWorldSizeCorrection = { 90,85 };

    float SoilEffectDuration = -1;
    float SoilEffectRenderTick = 0;
    int SoilEffectRenderRowAdd = 0; 
    std::vector<SOIL> SOILs;
    // ���� ���� ���� ����Ʈ �غ�
    void SOIL_EffectSetUp();
    void SOIL_EffectRender(HDC hdc, vec camera_pos);
    void SOIL_EffectEnd();
    void JumpAttackAlarm();
    // ���⿡ ���� ����

    int PatternTableNum;
    int CurrentPatternIdx = 0; 
    bool bAnimLoop = false;

    int CalcAttackAnimRowIdxFromDir();
    std::shared_ptr<class Bmp> AnimDirSpriteUpdate();

    void HitCalc(std::pair<int, int> AttackRange);
    void AttackStart();
    void AnimColUpdate();
    void AnimUpdateFromCurrentState();
    void StateTranslation();
    void UpdateDir();
    void StateSetUp(EState NewState, float Duration, float StateAnimDelta, int SetStartColIdx
    , bool SetAnimLoop, int AnimColMax);
    EState CurrentState = EState::IDLE;
    
    void DieAction();
    /////////////////////////// Pattern 
    void BoxAttack();
    void RotationBoxAttack(size_t NUM);
    void PillarMultipleAttack();
    void PillarSpiralAttack();
    void BoxDirectPillarAttack();
    ///////////////////////////

    /// <summary>
    // �ִϸ��̼� �������ӽð� ��� ���� �����ϰ� ���� ����
    // ���� ���ۿ� ���� �Լ�
    void BoxAttackStart();
    void RotationBoxAttackStart();
    void BoxDirectPillarAttackStart();
    void PillarMultipleAttackStart();
    void PillarSpiralAttackStart(); 
    // ���� ����� Attack ������� ����
    auto GetAttackAnimPakage();
    void AttackAnimPlay();
    /// </summary>
    
    void PillarPredictionAttack();
    void PillarCircle();
    // Ÿ���� ��������� ������ ��հ���
    void PillarDirectAttack();

    void ShockAttackStart();
    void ShockAttackEnd();
};

auto BOSS::GetAttackAnimPakage()
{
    static auto AttackAnimPackage = [this] {AttackAnimPlay(); return true; };
    return AttackAnimPackage;
}