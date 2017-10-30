#pragma once


class Actor;

class Pickable
{
public:
	virtual ~Pickable() {};

	bool pick(Actor * beingPicked, Actor * picking);
	virtual bool useConsume(Actor * beUsed, Actor * user);
};

