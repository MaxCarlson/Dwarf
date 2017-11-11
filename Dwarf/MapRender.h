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

	// Render whichever Map MapRender 
	// is associated with
	void render();

	Map & map;

	// Current vertical slice of map
	// we're rendering right now
	int currentZLevel;

	// Change z level of camera by one
	bool incrementZLevel(int inc);
	// Jump camera to z level
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

	friend class Map;
};

