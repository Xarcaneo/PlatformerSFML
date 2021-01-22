#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"

struct SharedContext{
	SharedContext():
		m_wind(nullptr),
		m_eventManager(nullptr),
		m_textureManager(nullptr)
		{}

	Window* m_wind;
	EventManager* m_eventManager;
	TextureManager* m_textureManager;
};