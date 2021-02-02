#include "Map.h"
#include "StateManager.h"

Map::Map(SharedContext* l_context, BaseState* l_currentState)
	:m_context(l_context), m_playerId(-1), m_currentState(l_currentState)
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

		std::string backgroundKey = "";
		backgroundKey = root["Config"][0]["Background"].asString();

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
	std::cout << "--- Map Loaded! ---" << std::endl;
}

void Map::Update(float l_dT){
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
	m_background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::Draw(){
	sf::RenderWindow* l_wind = m_context->m_wind->GetRenderWindow();
	sf::FloatRect viewSpace = m_context->m_wind->GetViewSpace();
	l_wind->draw(m_background);
}

void Map::PurgeMap(){
	m_context->m_entityManager->Purge();
}

void Map::PurgeTileSet(){
}