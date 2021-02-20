#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <BOX2D/box2d.h>
#include <json/json.h>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include "BaseState.h"
#include "SharedContext.h"

enum Sheet { Tile_Size = 32, Sheet_Width = 256, Sheet_Height = 256, Num_Layers = 4 };
using TileID = unsigned int;

struct TileInfo {
	TileInfo(SharedContext* l_context,
		const std::string& l_texture = "", TileID l_id = 0)
		: m_context(l_context), m_id(0)
	{
		TextureManager* tmgr = l_context->m_textureManager;
		if (l_texture == "") { m_id = l_id; return; }
		if (!tmgr->RequireResource(l_texture)) { return; }
		m_texture = l_texture;
		m_id = l_id;
		m_sprite.setTexture(*tmgr->GetResource(m_texture));
		sf::IntRect tileBoundaries(m_id % (Sheet::Sheet_Width / Sheet::Tile_Size) * Sheet::Tile_Size,
			m_id / (Sheet::Sheet_Height / Sheet::Tile_Size) * Sheet::Tile_Size,
			Sheet::Tile_Size, Sheet::Tile_Size);
		m_sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo() {
		if (m_texture == "") { return; }
		m_context->m_textureManager->ReleaseResource(m_texture);
	}

	sf::Sprite m_sprite;

	TileID m_id;
	std::string m_name;

	SharedContext* m_context;
	std::string m_texture;
};

struct Tile {
	TileInfo* m_properties;
};

using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;

class Map{
public:
	Map(SharedContext* l_context, BaseState* l_currentState);
	~Map();

	Tile* GetTile(unsigned int l_x, unsigned int l_y, unsigned int l_layer);

	unsigned int GetTileSize()const;
	int GetPlayerId()const;

	void LoadMap();
	void LoadTiles(const std::string& l_path);

	void Update(float l_dT);
	void Draw(unsigned int l_layer);
	void BackgroundDraw();

	void Execute();
	bool IsLoaded();
	//float GetCompletion();
private:
	// Method for converting 2D coordinates to 1D ints.
	unsigned int ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const;

	void PurgeMap();
	void PurgeTileSet();

	int m_playerId;
	SharedContext* m_context;
	sf::Sprite m_background;
	BaseState* m_currentState;

	sf::Vector2u m_maxMapSize;
	TileSet m_tileSet;
	TileMap m_tileMap;
	unsigned int m_tileCount;
	unsigned int m_tileSetCount;

	void RunTask();
	sf::Thread	m_thread;
	bool m_loaded;
	sf::Mutex m_mutex;
};