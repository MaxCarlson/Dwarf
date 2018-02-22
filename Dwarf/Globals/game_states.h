#pragma once


enum GameState
{
	NEW_GAME,
	LOAD_GAME,
	SAVE_GAME,
	QUIT_GAME,
	PLAYING,
	DESIGN,
	PAUSED
};

enum DesignStates
{
	ARCHITECURE,
	BUILD,
	CHOP_TREES,
	DEV_MODE,
	HARVEST,
	MINING,
	STOCKPILES,
	WORK_ORDERS,
	JOB_PREFERENCES,
	DWARF_INFO
};

extern GameState gameState;
extern DesignStates designState;

