#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* l_context)
	:m_context(l_context), m_playerId(-1)
{
	m_context->m_gameMap = this;
}

Map::~Map(){
	PurgeMap();
	PurgeTileSet();
	m_context->m_gameMap = nullptr;
}

int Map::GetPlayerId()const{ return m_playerId; }

void Map::LoadMap(const std::string& l_path){
	
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
			BodyDef.position = b2Vec2(converter::pixelsToMeters<float>(pos.x),
				converter::pixelsToMeters<float>(pos.y));

			b2Body* collisionBox;
			collisionBox = m_context->m_world->CreateBody(&BodyDef);
	
			b2PolygonShape box2d_shape;
			box2d_shape.SetAsBox(converter::pixelsToMeters<double>(size.x / 2.0),
				converter::pixelsToMeters<double>(size.y / 2.0));
			b2FixtureDef FixtureDef;
			//FixtureDef.density = m_density;
			//FixtureDef.friction = m_friction;
			//FixtureDef.restitution = m_restitution;
			FixtureDef.shape = &box2d_shape;
			collisionBox->CreateFixture(&FixtureDef);

			pos.x = converter::metersToPixels(collisionBox->GetPosition().x);
			pos.y = converter::metersToPixels(collisionBox->GetPosition().y);
		}
	}

	mapFile.close();
	std::cout << "--- Map Loaded! ---" << std::endl;
}

void Map::Update(float l_dT){
}

void Map::Draw(unsigned int l_layer){
	sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
}

void Map::PurgeMap(){
	m_context->m_entityManager->Purge();
}

void Map::PurgeTileSet(){
}