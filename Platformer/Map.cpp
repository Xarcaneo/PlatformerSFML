#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* l_context, BaseState* l_currentState)
	:m_context(l_context), m_playerId(-1), m_currentState(l_currentState), m_maxMapSize(0,0),
	 m_thread(&Map::LoadMap, this), m_loaded(false)
{
	m_context->m_gameMap = this;
	LoadTiles("tiles.cfg");
}

Map::~Map(){
	PurgeMap();
	PurgeTileSet();
	m_context->m_gameMap = nullptr;
}

Tile* Map::GetTile(unsigned int l_x, unsigned int l_y, unsigned int l_layer)
{
	if (l_x < 0 || l_y < 0 || l_x >= m_maxMapSize.x || l_y >= m_maxMapSize.y || l_layer < 0 || l_layer >= Sheet::Num_Layers)
	{
		return nullptr;
	}

	auto itr = m_tileMap.find(ConvertCoords(l_x, l_y, l_layer));
	if (itr == m_tileMap.end()) { return nullptr; }
	return itr->second;
}

int Map::GetPlayerId()const{ return m_playerId; }

void Map::LoadMap(){
	
	std::string l_path = "Assets/media/Maps/map1.json";

	Json::Value root;   // will contain the root value after parsing.
	Json::CharReaderBuilder builder;
	 
	std::ifstream mapFile(l_path);
	std::string errors;
	bool ok = Json::parseFromStream(builder, mapFile, &root, &errors);

	if (!ok)
	{
		// report to the user the failure and their locations in the document.
		std::cout << errors << "\n";
	}
	else
	{
		std::cout << "--- Loading a map: " << l_path << std::endl;

		std::string backgroundKey = "";
		backgroundKey = root["Config"][0]["Background"].asString();
		m_maxMapSize = { root["Config"][0]["MapSize"][0].asUInt() , root["Config"][0]["MapSize"][1].asUInt() };

		if (!m_context->m_textureManager->RequireResource(backgroundKey))
			backgroundKey = "";

		sf::Texture* texture = m_context->m_textureManager->GetResource(backgroundKey);
		m_background.setTexture(*texture);
		sf::Vector2f viewSize = m_currentState->GetView().getSize();
		sf::Vector2u textureSize = texture->getSize();
		sf::Vector2f scaleFactors;
		scaleFactors.x = viewSize.x / textureSize.x;
		scaleFactors.y = viewSize.y / textureSize.y;
		m_background.setScale(scaleFactors);

		//Static tiles
		for (int i = 0; i < root["STATIC"].size(); i++)
		{
			int tileId = 0;
			sf::Vector2i tileCoords;
			unsigned int tileLayer = 0;

			tileId = root["STATIC"][i]["TileID"].asInt();
			tileCoords = { root["STATIC"][i]["Coords"][0].asInt(),root["STATIC"][i]["Coords"][1].asInt() };
			tileLayer = root["STATIC"][i]["Layer"].asInt();

			if (tileId < 0) {
				std::cout << "! Bad tile id: " << tileId << std::endl;
				continue;
			}
			auto itr = m_tileSet.find(tileId);
			if (itr == m_tileSet.end()) {
				std::cout << "! Tile id(" << tileId << ") was not found in tileset." << std::endl;
				continue;
			}

			if (tileCoords.x > m_maxMapSize.x ||
				tileCoords.y > m_maxMapSize.y ||
				tileLayer >= Sheet::Num_Layers)
			{
				std::cout << "! Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}
			Tile* tile = new Tile();
			// Bind properties of a tile from a set.
			tile->m_properties = itr->second;

			if (!m_tileMap.emplace(ConvertCoords(
				tileCoords.x, tileCoords.y, tileLayer), tile).second)
			{
				// Duplicate tile detected!
				std::cout << "! Duplicate tile! : " << tileCoords.x << " " << tileCoords.y << std::endl;
				delete tile;
				continue;
			}
		}

		//Entities
		for (int i = 0; i < root["ENTITY"].size(); i++)
		{
			// Set up entity here.
			std::string name;
			unsigned int elevation;
			sf::Vector2f pos;

			name = root["ENTITY"][i]["Name"].asString();
			pos = { root["ENTITY"][i]["Position"][0].asFloat(), root["ENTITY"][i]["Position"][1].asFloat() };
			elevation = root["ENTITY"][i]["Elevation"].asUInt();

			if (name == "NPC/Player" && m_playerId != -1) { continue; }
			int entityId = m_context->m_entityManager->AddEntity(name);
			if (entityId < 0) { continue; }
			if (name == "NPC/Player") { m_playerId = entityId; }
			C_Position* position = m_context->m_entityManager->
				GetComponent<C_Position>(entityId, Component::Position);
			if (position) {
				position->SetElevation(elevation);
				position->SetPosition(pos); }
			C_Body* body = m_context->m_entityManager->
				GetComponent<C_Body>(entityId, Component::Body);
			if (body && position) {
				body->SetPosition(position->GetPosition());
			}
		}

		//Collision boxes
		for (int i = 0; i < root["COLLISION"].size(); i++)
		{
			sf::Vector2f pos, size;
			pos = { root["COLLISION"][i]["Position"][0].asFloat(), root["COLLISION"][i]["Position"][1].asFloat() };
			size = { root["COLLISION"][i]["Size"][0].asFloat(), root["COLLISION"][i]["Size"][1].asFloat() };

			b2BodyDef BodyDef;
			BodyDef.fixedRotation = true;
			BodyDef.type = b2_staticBody;
			BodyDef.position = b2Vec2(converter::pixelsToMeters<float>(pos.x + size.x / 2),
				converter::pixelsToMeters<float>(pos.y + size.y / 2));
			BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			b2Body* collisionBox;
			collisionBox = m_context->m_world->CreateBody(&BodyDef);
	
			b2PolygonShape box2d_shape;
			box2d_shape.SetAsBox(converter::pixelsToMeters<double>(size.x / 2.0),
				converter::pixelsToMeters<double>(size.y / 2.0));
			b2FixtureDef FixtureDef;
			FixtureDef.density = 0;
			FixtureDef.friction = 4;
			FixtureDef.restitution = 0;
			FixtureDef.shape = &box2d_shape;
			FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
			collisionBox->CreateFixture(&FixtureDef);

			pos.x = converter::metersToPixels(collisionBox->GetPosition().x);
			pos.y = converter::metersToPixels(collisionBox->GetPosition().y);
		}
	}

	mapFile.close();

	{
		sf::Lock lock(m_mutex);
		m_loaded = true;
	}

	std::cout << "--- Map Loaded! ---" << std::endl;
}

void Map::LoadTiles(const std::string& l_path) {
	std::ifstream tileSetFile;
	tileSetFile.open("Assets/" + l_path);
	if (!tileSetFile.is_open()) {
		std::cout << "! Failed loading tile set file: " << l_path << std::endl;
		return;
	}
	std::string line;
	while (std::getline(tileSetFile, line)) {
		if (line[0] == '|') { continue; }
		std::stringstream keystream(line);
		int tileId;
		std::string textureKey;
		keystream >> tileId >> textureKey;
		if (tileId < 0) { continue; }
		TileInfo* tile = new TileInfo(m_context, textureKey, tileId);
		keystream >> tile->m_name;
		if (!m_tileSet.emplace(tileId, tile).second) {
			// Duplicate tile detected!
			std::cout << "! Duplicate tile type: " << tile->m_name << std::endl;
			delete tile;
			tile = nullptr;
		}
	}
	tileSetFile.close();
}

void Map::Update(float l_dT){
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw(unsigned int l_layer){
	sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();

	sf::Vector2i tileBegin(floor(viewSpace.left / Sheet::Tile_Size), floor(viewSpace.top / Sheet::Tile_Size));
	sf::Vector2i tileEnd(ceil((viewSpace.left + viewSpace.width) / Sheet::Tile_Size),
		ceil((viewSpace.top + viewSpace.height) / Sheet::Tile_Size));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x) {
		for (int y = tileBegin.y; y <= tileEnd.y; ++y) {
			Tile* tile = GetTile(x, y, l_layer);
			if (!tile) { continue; }
			sf::Sprite& sprite = tile->m_properties->m_sprite;
			sprite.setPosition(x * Sheet::Tile_Size, y * Sheet::Tile_Size);
			l_wind->draw(sprite);
			++count;
		}
	}
}

void Map::BackgroundDraw()
{
	sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();
	l_wind->draw(m_background);
}

unsigned int Map::ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const
{
	return ((l_layer * m_maxMapSize.y + l_y) * m_maxMapSize.x + l_x);
}

void Map::PurgeMap() {
	while (m_tileMap.begin() != m_tileMap.end()) {
		delete m_tileMap.begin()->second;
		m_tileMap.erase(m_tileMap.begin());
	}
	m_tileCount = 0;
	m_context->m_entityManager->Purge();
}

void Map::PurgeTileSet() {
	while (m_tileSet.begin() != m_tileSet.end()) {
		delete m_tileSet.begin()->second;
		m_tileSet.erase(m_tileSet.begin());
	}
	m_tileSetCount = 0;
}

void Map::Execute()
{
	m_loaded = false;
	m_thread.launch();
}

bool Map::IsLoaded()
{
	sf::Lock lock(m_mutex);
	return m_loaded;
}

