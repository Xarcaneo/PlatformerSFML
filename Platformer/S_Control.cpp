#include "S_Control.h"
#include "System_Manager.h"

S_Control::S_Control(SystemManager* l_systemMgr)
	: S_Base(System::Control,l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Body);
	req.TurnOnBit((unsigned int)Component::Controller);
	m_requiredComponents.push_back(req);
	req.Clear();
}

S_Control::~S_Control(){}

void S_Control::Update(float l_dT){}

void S_Control::HandleEvent(const EntityId& l_entity, 
	const EntityEvent& l_event)
{
	switch(l_event){
	case EntityEvent::Moving_Left: MoveEntity(l_entity, Direction::Left); break;
	case EntityEvent::Moving_Right: MoveEntity(l_entity, Direction::Right); break;
	//case EntityEvent::Moving_Up: MoveEntity(l_entity, Direction::Up); break;
	}
}

void S_Control::Notify(const Message& l_message){}

void S_Control::MoveEntity(const EntityId& l_entity, 
	const Direction& l_dir)
{
	C_Body* body = m_systemManager->GetEntityManager()->GetComponent<C_Body>(l_entity, Component::Body);;
	body->Move(l_dir);
} 