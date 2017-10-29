#include "Destructible.h"
#include "Actor.h"
#include "Engine.h"
#include "Gui.h"

#include <iostream>


Destructible::Destructible(int maxHp, int defense, const char * corpseName) : maxHp(maxHp), hp(maxHp), defense(defense), corpseName(corpseName)
{
}

int Destructible::takeDamage(Actor * owner, int damage)
{
	damage -= defense;
	if (damage > 0) {
		hp -= damage;
		if (hp <= 0)
			die(owner);
	}
	else {
		damage = 0;
	}
	return damage;
}

void Destructible::die(Actor * owner)
{
	owner->ch = '%';
	owner->col = TCODColor::darkRed;
	strcpy_s(owner->name, corpseName);
	owner->blocks = false;
	engine.sendToBack(owner); // Draw corpses before living actors
}

MonsterDestructible::MonsterDestructible(int maxHp, int defense, const char * corpseName) : Destructible(maxHp, defense, corpseName)
{	
}

void MonsterDestructible::die(Actor * owner)
{
	engine.gui->message(TCODColor::lightGrey, "%s is dead\n", owner->name);
	Destructible::die(owner);
}

PlayerDestructible::PlayerDestructible(int maxHp, int defense, const char * corpseName) : Destructible(maxHp, defense, corpseName)
{
}

void PlayerDestructible::die(Actor * owner)
{
	engine.gui->message(TCODColor::red, "You Died!\n");
	Destructible::die(owner);
	engine.gameStatus = Engine::DEFEAT;
}
