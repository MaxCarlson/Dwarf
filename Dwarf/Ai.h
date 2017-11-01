#pragma once

class Actor;

class Ai
{
public:
	virtual ~Ai() {};
	virtual void update(Actor * owner) = 0;
};

class PlayerAi : public Ai {
public:
	void update(Actor * owner);

protected:
	bool move(Actor * owner, int targetX, int targetY);
};

class MonsterAi : public Ai
{
public: 
	//void update(Actor * owner);

protected:
	int oppMoveCount;
	//void moveOrAttack(Actor * owner, int targetX, int targetY);
};
