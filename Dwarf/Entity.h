#pragma once

#include <inttypes.h>

typedef uint32_t Id;

class GameObject
{

};

class Entity : GameObject
{
public:
	Entity(Id eId) : eId(eId) {};
	Id eId;
};

