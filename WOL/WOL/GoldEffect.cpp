#include "pch.h"
#include "GoldEffect.h"
#include "player.h"
#include "object_mgr.h"
#include "player.h"
#include "player_info.h"
#include "object_mgr.h"

std::shared_ptr<GoldEffect> GoldEffect::MakeGold(int ImgLocationX, int ImgLocationY, 
    std::wstring ImgKey, layer_type layer_ID, int AnimColNum,
    int AnimRowIndex, float Duration, float AnimDuration,
    int PaintSizeX, int PaintSizeY, float ScaleX, float ScaleY,
    std::weak_ptr<class object> _Owner)
{
    auto _gold = object_mgr::instance().insert_object<GoldEffect>(ImgLocationX, ImgLocationY,
        ImgKey, layer_ID, AnimColNum,
        AnimRowIndex, Duration, AnimDuration, PaintSizeX, PaintSizeY, ScaleX, ScaleY);

    _gold->_transform->_location = { ImgLocationX, ImgLocationY };
    _gold->_owner = _Owner;

    return _gold;
}
void GoldEffect::initialize()
{
    Effect::initialize();

    Gold = math::Rand<int>({ 20,40 });

    if (!_transform)return;
}

Event GoldEffect::update(float dt)
{
    Event _event = Effect::update(dt);
    initLife -= dt;
    if (initLife > 0)return _event;

    auto sp_player = std::dynamic_pointer_cast<Player>(_owner.lock());
    if (!sp_player) return Event::Die;
    if (!sp_player->_transform) _event = Event::Die;

    vec dis = sp_player->_transform->_location - _transform->_location;

    if (dis.length() < distance)
    {
        sp_player->_player_info->AddGold(Gold);
        

        std::wstring str = L"+";
        str += std::to_wstring(Gold);

        vec randvec = math::RandVec(); 
        randvec.y = (abs(randvec.y));
        object_mgr::instance().TextEffectMap[RGB(223, 207, 0)].
            push_back({ _transform->_location,randvec,
               1.f,Gold,str });

        _event = Event::Die;
    }

    _transform->_dir = dis.get_normalize();
    _transform->Move(_transform->_dir, followSpeed* dt);

    ImgLocationX = _transform->_location.x;
    ImgLocationY = _transform->_location.y;

    return _event; 
}
