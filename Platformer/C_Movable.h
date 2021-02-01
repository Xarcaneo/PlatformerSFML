#pragma once
#include "C_Base.h"
#include "Directions.h"
#include <SFML/System/Vector2.hpp>

class C_Movable : public C_Base {
public:
	C_Movable() : C_Base(Component::Movable),
		m_direction((Direction)0)
	{}

	void ReadIn(std::stringstream& l_stream) {
		l_stream >> m_speed;

		unsigned int dir = 0;
		l_stream >> dir;
		m_direction = (Direction)dir;
	}

	const float& GetSpeed() { return m_speed; }
	Direction GetDirection() { return m_direction; }

	void SetDirection(const Direction& l_dir) { m_direction = l_dir; }


private:
	float m_speed;
	Direction m_direction;
};