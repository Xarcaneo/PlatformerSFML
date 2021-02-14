#pragma once
#include "C_Base.h"
#include <SFML/Graphics/Rect.hpp>

class C_Collidable : public C_Base{
public:
	C_Collidable() : C_Base(Component::Collidable){}

	void ReadIn(std::stringstream& l_stream){
		unsigned int origin = 0;
		l_stream >> m_AABB.width >> m_AABB.height >> m_offset.x
			>> m_offset.y;
	}

	const sf::FloatRect& GetCollidable(){ return m_AABB; }

	void SetCollidable(const sf::FloatRect& l_rect){ m_AABB = l_rect; }

	void SetSize(const sf::Vector2f& l_vec){
		m_AABB.width = l_vec.x;
		m_AABB.height = l_vec.y;
	}

	void SetPosition(const sf::Vector2f& l_vec){
		m_AABB.left = l_vec.x - (m_AABB.width / 2) + m_offset.x;
		m_AABB.top = l_vec.y - (m_AABB.height / 2) + m_offset.y;
	}
private:
	sf::FloatRect m_AABB;
	sf::Vector2f m_offset;
};