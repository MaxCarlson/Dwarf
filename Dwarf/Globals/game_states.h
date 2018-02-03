#pragma once


enum GameState
{
	NEW_GAME,
	LOAD_GAME,
	SAVE_GAME,
	QUIT_GAME,
	PLAYING,
	DESIGN,
};

enum DesignStates
{
	ARCHITECURE,
	BUILD,
	CHOP_TREES,
	HARVEST,
	MINING,
	STOCKPILES,
	WORK_ORDERS
};

extern GameState gameState;
extern DesignStates designState;

