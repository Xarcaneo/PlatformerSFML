#include "State_Loading.h"
#include "StateManager.h"

#pragma once

State_Loading::State_Loading(StateManager* l_stateManager)
	:BaseState(l_stateManager)
{
}

State_Loading::~State_Loading()
{
}

void State_Loading::OnCreate()
{
	sf::Vector2u size = m_stateMgr->GetContext()->m_wind->GetWindowSize();
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_view.zoom(0.6f);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);

	this->m_gameMap = new Map(m_stateMgr->GetContext(), this);
	this->m_gameMap->Execute();
	m_stateMgr->GetContext()->m_gameMap = this->m_gameMap;
	m_stateMgr->GetContext()->m_loadingProperties->m_loading = false;

	m_view.reset(sf::FloatRect(0,0,size.x,size.y));
	m_view.setSize(size.x, size.y);
	m_view.setCenter(size.x / 2, size.y / 2);
	m_stateMgr->GetContext()->m_wind->GetRenderWindow()->setView(m_view);

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->RequireResource("Loading");
	m_loadingSprite.setTexture(*textureMgr->GetResource("Loading"));
	m_loadingSprite.setOrigin(textureMgr->GetResource("Loading")->getSize().x / 2.0f,
							textureMgr->GetResource("Loading")->getSize().y / 2.0f);

	m_loadingSprite.setPosition(size.x / 2.0f, size.y / 2.0f);

	auto context = m_stateMgr->GetContext();
	context->m_fontManager->RequireResource("Main");
	m_text.setFont(*context->m_fontManager->GetResource("Main"));
	m_text.setCharacterSize(14);
	m_text.setStyle(sf::Text::Bold);
	m_text.setString("Press space to continue");
	sf::FloatRect textRect = m_text.getLocalBounds();
	m_text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	m_text.setPosition(m_loadingSprite.getPosition().x,
		m_loadingSprite.getPosition().y + textureMgr->GetResource("Loading")->getSize().y / 1.5f);
}

void State_Loading::OnDestroy()
{
	auto context = m_stateMgr->GetContext();

	TextureManager* textureMgr = m_stateMgr->GetContext()->m_textureManager;
	textureMgr->ReleaseResource("Loading");
}

void State_Loading::Activate()
{
}

void State_Loading::Deactivate()
{
}

void State_Loading::Draw()
{
	sf::RenderWindow* window = m_stateMgr->
		GetContext()->m_wind->GetRenderWindow();

	window->draw(m_loadingSprite);

	if (m_gameMap->IsLoaded())
		window->draw(m_text);
}

void State_Loading::Update(const sf::Time& l_time)
{
	if (m_gameMap->IsLoaded())
		m_stateMgr->SwitchTo(StateType::Game);
}
