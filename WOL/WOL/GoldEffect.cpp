#include "pch.h"
#include "GoldEffect.h"
#include "player.h"
#include "object_mgr.h"
#include "player.h"
#include "player_info.h"

void GoldEffect::initialize()
{
    Effect::initialize();

    Gold = math::Rand<int>({ 1,30 });
}

Event GoldEffect::update(float dt)
{
    Event _event = Effect::update(dt);
    initLife -= dt;
    auto sp_player = std::dynamic_pointer_cast<Player>(_owner.lock());
    if (!sp_player) _event = Event::Die;
    if (!sp_player->_transform) _event = Event::Die;

    vec dis = sp_player->_transform->_location - _transform->_location;

    if (dis.length() < distance && initLife <0.f)
    {
        sp_player->_player_info->gold += Gold;
        
        _event = Event::Die;
    }

    _transform->_dir = dis.get_normalize();
    _transform->Move(_transform->_dir, followSpeed* dt);

    ImglocationX = _transform->_location.x;
    ImgLocationY = _transform->_location.y;

    return _event; 
}
