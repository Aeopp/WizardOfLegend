#pragma once
#include "moveable_object.h"

class bullet :
    public moveable_object
{
public:
    using super = moveable_object;
public:
    virtual void update()override;
    virtual void initialize()override;
};

