#pragma once
#include "BaseState.h"
#include "EventManager.h"
#include "Map.h"

class State_Loading : public BaseState {
public:
	State_Loading(StateManager* l_stateManager);
	~State_Loading();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& l_time);
	void Draw();
private:
	sf::Text m_text;
	sf::Sprite m_loadingSprite;
	Map* m_gameMap;
};