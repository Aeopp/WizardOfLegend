#pragma once
#include "object.h"
class actor :
    public object
{
public:
    float _speed{300.f};
    vec _dir;
};

