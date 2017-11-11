#pragma once

class TCODMap;
class Map;

// Sepperate render logic from map 
// generation. Currently performs rendering for local map
class MapRender
{
public:
	MapRender(Map & map);
	~MapRender();

	void render();

	Map & map;

	// Current vertical slice of map
	// we're rendering right now
	int currentZLevel;

	bool incrementZLevel(int inc);
	void jumpToZLevel(int level);

	// Top left coordinates of camera
	// In other words, camera offset from topLeft
	int offsetX = 0, offsetY = 0;

	enum CamDirections
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	void moveCamera(const CamDirections D);

private:
	// Width and height of current window
	// displaying game
	int panelWidth, panelHeight;

	// Holds the map the camera will render
	TCODMap * renderMap;	

	friend class Map;
};

