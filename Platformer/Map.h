#pragma once
#include <SFML/Graphics.hpp>
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
	const sf::Vector2u& GetMapSize()const;
	const sf::Vector2f& GetPlayerStart()const;
	int GetPlayerId()const;

	void LoadMap(const std::string& l_path);

	void Update(float l_dT);
	void Draw(unsigned int l_layer);
private:
	// Method for converting 2D coordinates to 1D ints.
	unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;

	void LoadTiles(const std::string& l_path);

	void PurgeMap();
	void PurgeTileSet();

	sf::Vector2u m_maxMapSize;
	sf::Vector2f m_playerStart;
	int m_playerId;
	unsigned int m_tileCount;
	unsigned int m_tileSetCount;
	SharedContext* m_context;
};