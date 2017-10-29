#pragma once

class Actor;

class Attacker
{
public:
	int power;

	Attacker(int power);
	void attack(Actor * owner, Actor * target);
};

