#pragma once
#include <SFML/Graphics.hpp>
#include <BOX2D/box2d.h>
#include <json/json.h>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include "SharedContext.h"

enum Sheet { Tile_Size = 32, Sheet_Width = 256, Sheet_Height = 256, Num_Layers = 4 };

class Map{
public:
	Map(SharedContext* l_context);
	~Map();

	unsigned int GetTileSize()const;
	int GetPlayerId()const;

	void LoadMap(const std::string& l_path);

	void Update(float l_dT);
	void Draw(unsigned int l_layer);
private:
	// Method for converting 2D coordinates to 1D ints.
	unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;

	void PurgeMap();
	void PurgeTileSet();

	int m_playerId;
	SharedContext* m_context;
};