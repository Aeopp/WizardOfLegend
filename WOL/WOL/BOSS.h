#pragma once
#include "object.h"
#include "Shadow.h"
#include "SOIL.h"

struct PatternInfo
{
    float AnimDelta;
    float Duration;
    std::function<void()> Skill;
};
class BOSS : public object
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
       // 박스 3개
       BOXATTACK,
       // 박스 6개
       ROTBOXATTACK,
       // 부동자세에서 플레이어 방향 기둥 공격
       PILLARATTACK,
       // 느린 점프 이후 주변 기둥 소환 공격
       JUMPPILLARATTACK, 
       // 박스를 던지며 기둥 소환하는 공격
        BOXPILLARATTACK , 
        None  ,
    };

    std::unordered_map<int, PatternInfo> PatternMap;


    void initialize()override;
    void render(HDC hdc, vec camera_pos, vec size_factor)override;
    Event update(float dt)override;
    void Hit(std::weak_ptr<object> _target)override;
    void SetUp(std::weak_ptr<class object> AttackTarget, vec Location);
    virtual ~BOSS()noexcept;


    //left right 스프라이트
    std::pair< std::shared_ptr<class Bmp>, std::shared_ptr<class Bmp> > sp_Bmps;
    std::shared_ptr<class Bmp> sp_AttackBmp;
    std::shared_ptr<class Bmp> sp_SOILBmp;

    std::shared_ptr<class Bmp >CurrentBmp; 


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
    // Left Right 스프라이트 테이블
    std::vector<uint32_t> CurrentAnimColNumTalble{ 1,9,2,8,2,1 };
    // Attack 스프라이트 테이블
    std::vector<uint32_t> CurrentAttackAnimColNumTable{ 4,4,4,4 };
    bool bAttackAnimSprite = false;
    void AttackAnimOn();
    void AttackAnimEnd();
    int CurrentAnimColMax = 0;

    float SoilEffectDuration = -1;
    float SoilEffectRenderTick = 0;
    int SoilEffectRenderRowAdd = 0; 
    std::vector<SOIL> SOILs;
    // 점프 공격 이후 이펙트 준비
    void SOIL_EffectSetUp();
    void SOIL_EffectRender(HDC hdc, vec camera_pos);
    void SOIL_EffectEnd();

    // 여기에 패턴 세팅
    std::vector<int> PatternTable{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    int PatternTableNum;
    int CurrentPatternIdx = 0; 
    bool bAnimLoop = false;

    int CalcAttackAnimRowIdxFromDir();
    std::shared_ptr<class Bmp> AnimDirSpriteUpdate();

    int CalcAnimRowFromPattern();
    void HitCalc(std::pair<int, int> AttackRange);
    void AttackStart();
    void AnimColUpdate();
    void AnimUpdateFromCurrentState();
    void StateTranslation();
    void UpdateDir();
    void StateSetUp(EState NewState, float Duration, float StateAnimDelta);
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
    void BoxAttackStart();
    /// </summary>
    
    void PillarPredictionAttack();
    


    void PillarCircle();
    
    

    // 타겟의 진행방향을 예상해 기둥공격
    
    void PillarDirectAttack();
};

