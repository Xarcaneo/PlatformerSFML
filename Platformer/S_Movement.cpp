#include "S_Movement.h"
#include "System_Manager.h"
#include "Map.h"

S_Movement::S_Movement(SystemManager* l_systemMgr)
	: S_Base(System::Movement, l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::Movable);
	req.TurnOnBit((unsigned int)Component::Body);

	m_requiredComponents.push_back(req);
	req.Clear();

	m_systemManager->GetMessageHandler()->Subscribe(EntityMessage::Is_Moving, this);
}

S_Movement::~S_Movement() {}

void S_Movement::Update(float l_dT) {
	EntityManager* entities = m_systemManager->GetEntityManager();
	for (auto& entity : m_entities)
	{
		C_Body* body = entities->GetComponent<C_Body>(entity, Component::Body);

		body->Move();
		body->SetController(Controller::Stop);
	}
}

void S_Movement::HandleEvent(const EntityId& l_entity,
	const EntityEvent& l_event)
{ 
	switch (l_event) {
	case EntityEvent::Moving_Left: SetDirection(l_entity, Direction::Left); break;
	case EntityEvent::Moving_Right: SetDirection(l_entity, Direction::Right); break;
	}
}

void S_Movement::Notify(const Message& l_message) {
}

void S_Movement::SetDirection(const EntityId& l_entity,
	const Direction& l_dir)
{
	C_Movable* movable = m_systemManager->GetEntityManager()->GetComponent<C_Movable>(l_entity, Component::Movable);
	movable->SetDirection(l_dir);
	Message msg((MessageType)EntityMessage::Direction_Changed);
	msg.m_receiver = l_entity;
	msg.m_int = (int)l_dir;
	m_systemManager->GetMessageHandler()->Dispatch(msg);
}