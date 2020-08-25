#pragma once
#include "singleton_interface.h"

class collision_manager :
    public singleton_interface<collision_manager>
{
public:
    void collision_beetween_objects(
        const std::list<std::shared_ptr<class object>>& lhs,
        const std::list<std::shared_ptr<class object>>& rhs)const&;
};

