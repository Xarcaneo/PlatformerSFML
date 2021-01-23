#include "State_Game.h"
#include "StateManager.h"

State_Game::State_Game(StateManager* l_stateManager)
	: BaseState(l_stateManager){}

State_Game::~State_Game(){}

void State_Game::OnCreate(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	
	evMgr->AddCallback(StateType::Game,"Key_Escape",&State_Game::MainMenu,this);
	evMgr->AddCallback(StateType::Game, "Key_O", &State_Game::ToggleOverlay, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveLeft", &State_Game::PlayerMove, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveRight", &State_Game::PlayerMove, this);
	evMgr->AddCallback(StateType::Game, "Player_MoveUp", &State_Game::PlayerMove, this);

	sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	m_view.setSize(size.x,size.y);
	m_view.setCenter(size.x/2,size.y/2);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);

	m_world = m_stateMgr->GetContext()->m_world;

	m_gameMap = new Map(m_stateMgr->GetContext()/*, this*/);
	m_gameMap->LoadMap("Assets/media/Maps/map1.map");

	EntityManager* entities = m_stateMgr->GetContext()->m_entityManager;
	//m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Collision>(System::Collision)->SetMap(m_gameMap);
	//m_stateMgr->GetContext()->m_systemManager->GetSystem<S_Movement>(System::Movement)->SetMap(m_gameMap);
	m_player = m_gameMap->GetPlayerId();
}

void State_Game::OnDestroy(){
	EventManager* evMgr = m_stateMgr->
		GetContext()->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveUp");
	
	delete m_gameMap;
	m_gameMap = nullptr;
}

void State_Game::Update(const sf::Time& l_time){
	SharedContext* context = m_stateMgr->GetContext();
	m_world->Step(l_time.asSeconds(), 6, 2);
	UpdateCamera();
	m_gameMap->Update(l_time.asSeconds());
	context->m_systemManager->Update(l_time.asSeconds());
}

void State_Game::UpdateCamera(){
	if (m_player == -1){ return; }
	SharedContext* context = m_stateMgr->GetContext();
	C_Position* pos = m_stateMgr->GetContext()->m_entityManager->
		GetComponent<C_Position>(m_player, Component::Position);

	m_view.setCenter(pos->GetPosition());
	context->m_wind->GetRenderWindow()->setView(m_view);
}

void State_Game::Draw(){
	for(unsigned int i = 0; i < Sheet::Num_Layers; ++i){
		m_gameMap->Draw(i);
		m_stateMgr->GetContext()->m_systemManager->Draw(
			m_stateMgr->GetContext()->m_wind, i);
	}
}

void State_Game::MainMenu(EventDetails* l_details){
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Pause(EventDetails* l_details){
	m_stateMgr->SwitchTo(StateType::Paused);
}

void State_Game::Activate(){}

void State_Game::Deactivate(){}

void State_Game::PlayerMove(EventDetails* l_details){
	Message msg((MessageType)EntityMessage::Move);
	if (l_details->m_name == "Player_MoveLeft"){
		msg.m_int = (int)Direction::Left;
	} else if (l_details->m_name == "Player_MoveRight"){
		msg.m_int = (int)Direction::Right;
	} else if (l_details->m_name == "Player_MoveUp"){
		msg.m_int = (int)Direction::Up;
	}
	msg.m_receiver = m_player;
	m_stateMgr->GetContext()->m_systemManager->GetMessageHandler()->Dispatch(msg);
}

void State_Game::ToggleOverlay(EventDetails* l_details){
	m_stateMgr->GetContext()->m_debugOverlay.SetDebug(!m_stateMgr->GetContext()->m_debugOverlay.Debug());
}