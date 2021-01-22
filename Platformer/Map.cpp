#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* l_context)
	:m_context(l_context), m_maxMapSize(32, 32), m_playerId(-1)
{
	m_context->m_gameMap = this;
	LoadTiles("tiles.cfg");
}

Map::~Map(){
	PurgeMap();
	PurgeTileSet();
	m_context->m_gameMap = nullptr;
}

const sf::Vector2u& Map::GetMapSize()const{ return m_maxMapSize; }
const sf::Vector2f& Map::GetPlayerStart()const{ return m_playerStart; }
int Map::GetPlayerId()const{ return m_playerId; }

void Map::LoadMap(const std::string& l_path){
	std::ifstream mapFile;
	mapFile.open(l_path);
	if (!mapFile.is_open()){
		std::cout << "! Failed loading map file: " << l_path << std::endl;
		return;
	}
	std::string line;
	std::cout << "--- Loading a map: " << l_path << std::endl;
	while(std::getline(mapFile,line)){
		if (line[0] == '|'){ continue; }
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;

		if (type == "SIZE") {
			keystream >> m_maxMapSize.x >> m_maxMapSize.y;
		}
		else if(type == "ENTITY"){
			// Set up entity here.
			std::string name;
			keystream >> name;
			if (name == "Player" && m_playerId != -1){ continue; }
			int entityId = m_context->m_entityManager->AddEntity(name);
			if (entityId < 0){ continue; }
			if(name == "Player"){ m_playerId = entityId; }
			C_Base* position = m_context->m_entityManager->
				GetComponent<C_Position>(entityId,Component::Position);
			if(position){ keystream >> *position; }
		} else {
			// Something else.
			std::cout << "! Unknown type \"" << type << "\"." << std::endl;
		}
	}
	mapFile.close();
	std::cout << "--- Map Loaded! ---" << std::endl;
}

void Map::LoadTiles(const std::string& l_path){

}

void Map::Update(float l_dT){}

void Map::Draw(unsigned int l_layer){
	sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
}

unsigned int Map::ConvertCoords(unsigned int l_x, unsigned int l_y, unsigned int l_layer)const
{
	return ((l_layer*m_maxMapSize.y+l_y) * m_maxMapSize.x + l_x);
}

void Map::PurgeMap(){
	m_context->m_entityManager->Purge();
}

void Map::PurgeTileSet(){
}