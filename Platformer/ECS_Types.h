#pragma once
using ComponentType = unsigned int;
#define N_COMPONENT_TYPES 32

enum class Component{
	Position = 0, SpriteSheet, SingleSprite, State, Controller, Body, Movable
};

enum class System{
	Renderer = 0, Control, State, SheetAnimation, Movement
};