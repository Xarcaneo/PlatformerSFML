#include "S_CharacterUI.h"
#include "System_Manager.h"

S_CharacterUI::S_CharacterUI(SystemManager* l_systemMgr)
	: S_Base(System::Character_UI, l_systemMgr)
{
	Bitmask req;
	req.TurnOnBit((unsigned int)Component::Position);
	req.TurnOnBit((unsigned int)Component::Body);
	req.TurnOnBit((unsigned int)Component::UI_Element);
	req.TurnOnBit((unsigned int)Component::Health);
	m_requiredComponents.push_back(req);
	req.ClearBit((unsigned int)Component::Health);
	m_requiredComponents.push_back(req);
}

S_CharacterUI::~S_CharacterUI(){
	m_textureManager->ReleaseResource("HeartBar");
}

void S_CharacterUI::Update(float l_dT){}
void S_CharacterUI::HandleEvent(const EntityId& l_entity, const EntityEvent& l_event){}
void S_CharacterUI::Notify(const Message& l_message){}

void S_CharacterUI::Render(Window* l_wind)
{
	EntityManager* entities = m_systemManager->GetEntityManager();
	for (auto &entity : m_entities){
		C_Health* health = entities->GetComponent<C_Health>(entity, Component::Health);
		C_Position* pos = entities->GetComponent<C_Position>(entity, Component::Position);
		C_Body* body = entities->GetComponent<C_Body>(entity, Component::Body);
		C_UI_Element* ui = entities->GetComponent<C_UI_Element>(entity, Component::UI_Element);
		if (health){
			pos->SetPosition(body->GetPosition());
			m_heartBar.setTextureRect(sf::IntRect(0, 0, m_heartBarSize.x * health->GetHealth(), m_heartBarSize.y));
			m_heartBar.setOrigin((m_heartBarSize.x * health->GetHealth()) / 2, m_heartBarSize.y);
			m_heartBar.setPosition(pos->GetPosition() + ui->GetOffset());
			l_wind->GetRenderWindow()->draw(m_heartBar);
		}
	}
}

void S_CharacterUI::SetUp(TextureManager* l_textureManager)
{
	m_textureManager = l_textureManager;
	m_textureManager->RequireResource("HeartBar");
	sf::Texture* txtr = m_textureManager->GetResource("HeartBar");
	txtr->setRepeated(true);
	m_heartBarSize = txtr->getSize();
	m_heartBar.setTexture(*txtr);
	m_heartBar.setScale(0.5f, 0.5f);
	m_heartBar.setOrigin(m_heartBarSize.x / 2, m_heartBarSize.y);
}
