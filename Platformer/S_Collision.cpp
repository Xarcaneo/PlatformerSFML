#include "S_Collision.h"
#include "System_Manager.h"

S_Collision::S_Collision(SystemManager* l_systemMgr)
	: S_Base(System::Collision, l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Body);
	req.TurnOnBit((unsigned int)Component::Collidable);
	m_requiredComponents.push_back(req);
	req.Clear();

	m_gameMap = nullptr;
}

S_Collision::~S_Collision(){}

void S_Collision::SetMap(Map* l_map){ m_gameMap = l_map; }

void S_Collision::Update(float l_dT){
	EntityManager* entities = m_systemManager->GetEntityManager();
	for (auto& entity : m_entities) {
		C_Position* position = entities->GetComponent<C_Position>(entity, Component::Position);
		C_Collidable* collidable = entities->GetComponent<C_Collidable>(entity, Component::Collidable);
		C_Body* body = entities->GetComponent<C_Body>(entity, Component::Body);

		collidable->SetPosition(body->GetPosition());
	}
	EntityCollisions();
}

void S_Collision::EntityCollisions(){
	EntityManager* entities = m_systemManager->GetEntityManager();
	for (auto itr = m_entities.begin(); itr != m_entities.end(); ++itr)
	{
		for (auto itr2 = std::next(itr); itr2 != m_entities.end(); ++itr2){
			C_Collidable* collidable1 = entities->GetComponent<C_Collidable>(*itr, Component::Collidable);
			C_Collidable* collidable2 = entities->GetComponent<C_Collidable>(*itr2, Component::Collidable);
			if(collidable1->GetCollidable().intersects(collidable2->GetCollidable()))
			{
				// Entity-on-entity collision!
			}
			C_Attacker* attacker1 = entities->GetComponent<C_Attacker>(*itr, Component::Attacker);
			C_Attacker* attacker2 = entities->GetComponent<C_Attacker>(*itr2, Component::Attacker);
			if (!attacker1 && !attacker2){ continue; }
			Message msg((MessageType)EntityMessage::Being_Attacked);
			if (attacker1){
				if (attacker1->GetAreaOfAttack().intersects(collidable2->GetCollidable())){
					// Attacker-on-entity collision!
					msg.m_receiver = *itr2;
					msg.m_sender = *itr;
					m_systemManager->GetMessageHandler()->Dispatch(msg);
				}
			}
			if (attacker2){
				if (attacker2->GetAreaOfAttack().intersects(collidable1->GetCollidable())){
					// Attacker-on-entity collision!
					msg.m_receiver = *itr;
					msg.m_sender = *itr2;
					m_systemManager->GetMessageHandler()->Dispatch(msg);
				}
			}
		}
	}
}

void S_Collision::HandleEvent(const EntityId& l_entity,const EntityEvent& l_event){}
void S_Collision::Notify(const Message& l_message){}