#pragma once
#include "ECS\Component.h"

struct Thoughts : public Component
{

};


CEREAL_REGISTER_TYPE(Thoughts);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Thoughts);