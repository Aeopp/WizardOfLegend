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
       // �ڽ� 3��
       BOXATTACK,
       // �ڽ� 6��
       ROTBOXATTACK,
       // �ε��ڼ����� �÷��̾� ���� ��� ����
       PILLARATTACK,
       // ���� ���� ���� �ֺ� ��� ��ȯ ����
       JUMPPILLARATTACK, 
       // �ڽ��� ������ ��� ��ȯ�ϴ� ����
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


    //left right ��������Ʈ
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
    // Left Right ��������Ʈ ���̺�
    std::vector<uint32_t> CurrentAnimColNumTalble{ 1,9,2,8,2,1 };
    // Attack ��������Ʈ ���̺�
    std::vector<uint32_t> CurrentAttackAnimColNumTable{ 4,4,4,4 };
    bool bAttackAnimSprite = false;
    void AttackAnimOn();
    void AttackAnimEnd();
    int CurrentAnimColMax = 0;

    float SoilEffectDuration = -1;
    float SoilEffectRenderTick = 0;
    int SoilEffectRenderRowAdd = 0; 
    std::vector<SOIL> SOILs;
    // ���� ���� ���� ����Ʈ �غ�
    void SOIL_EffectSetUp();
    void SOIL_EffectRender(HDC hdc, vec camera_pos);
    void SOIL_EffectEnd();

    // ���⿡ ���� ����
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
    
    

    // Ÿ���� ��������� ������ ��հ���
    
    void PillarDirectAttack();
};

