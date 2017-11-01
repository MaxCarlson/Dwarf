#pragma once
#include "include\libtcod.hpp"
#include "Tile.h"

static const int MAX_NAME_LENGTH = 20;

class Actor;
class Attacker;
class Destructible;
class Ai;
class Pickable;
class Container;

enum ActorId 
{
	PLAYER_ID,
	STONE_ID,
	STAIRS_ID,
};

class Actor
{
public:
	Actor(Coordinates co, int ch, int a_id, const char * name, const TCODColor & col);
	~Actor();

	void render() const;
	void update();

	Coordinates co;              // X, Y, Z location
	int ch;	                     // Symbol used during render	
	int a_id;				     // ID used to detmine identity of virtual classes 
	char name[MAX_NAME_LENGTH];  // Change to std::string eventually and and inline std::string to const char * method for using libtcod funcitons
	TCODColor col;
	bool blocks;				 // Does block movement?
	Attacker *attacker;			 // Can actor attack?
	Destructible * destructible; // Can actor be destroyed?
	Ai *ai;						 // actor has ai
	Pickable  * pickable;		 // Can we pick the actor/object up? // Add subset to class wearable? Object can be worn after being picked up?
	Container * container;       // Can actor/object store things?
};

class Stairs : Actor
{
public:
	Stairs();
};

class Stone : public Actor
{
public:
	Stone(Coordinates co, int ch, int a_id, const char * name, const TCODColor & col, int type) : Actor(co, ch, a_id, name, col), type(type) {};

	int type;

	enum StoneT {
		GRANITE,
		MARBLE,
		SLATE
	};

	enum StoneChar 
	{
		GRANITE_CH = '▓',
		MARBLE_CH = '▓',
		SLATE_CH = '%'
	};
};