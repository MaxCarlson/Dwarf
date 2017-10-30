#pragma once

class Actor;

class Destructible
{
public:
	virtual ~Destructible() {};
	int maxHp;
	int hp;
	int defense;
	const char *corpseName;

	Destructible(int maxHp, int defense, const char * corpseName);
	inline bool isDead() { return hp <= 0; }

	int takeDamage(Actor * owner, int damage);
	virtual void die(Actor * owner);
	int heal(int amount);
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
