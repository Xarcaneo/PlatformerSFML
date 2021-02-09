#pragma once
#include <BOX2D/box2d.h>
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "System_Manager.h"
#include "AudioManager.h"
#include "SoundManager.h"
#include "Entity_Manager.h"
#include "DebugOverlay.h"

class Map;

struct SharedContext{
	SharedContext():
		m_wind(nullptr),
		m_eventManager(nullptr),
		m_textureManager(nullptr),
		m_systemManager(nullptr),
		m_audioManager(nullptr),
		m_soundManager(nullptr),
		m_entityManager(nullptr),
		m_gameMap(nullptr),
		m_world(new b2World(b2Vec2(0, 9.81f))){}

	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
	SystemManager* m_systemManager;
	EntityManager* m_entityManager;
	AudioManager* m_audioManager;
	SoundManager* m_soundManager;
	Map* m_gameMap;
	b2World* m_world;
	DebugOverlay m_debugOverlay;
};