#pragma once

class Actor;

class Destructible
{
public:
	int maxHp;
	int hp;
	int defense;
	const char *corpseName;

	Destructible(int maxHp, int defense, const char * corpseName);
	inline bool isDead() { return hp <= 0; }

	int takeDamage(Actor * owner, int damage);
	virtual void die(Actor * owner);
};

class MonsterDestructible : public Destructible 
{
public:
	MonsterDestructible(int maxHp, int defense, const char * corpseName);
	void die(Actor * owner);
};

class PlayerDestructible : public Destructible
{
public:
	PlayerDestructible(int maxHp, int defense, const char * corpseName);
	void die(Actor * owner);
};
