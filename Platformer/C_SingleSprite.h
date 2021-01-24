#pragma once
#include "C_Drawable.h"
#include "TextureManager.h"

class C_SingleSprite : public C_Drawable {
public:
	C_SingleSprite() : C_Drawable(Component::SingleSprite) {}
	~C_SingleSprite() {}

	void ReadIn(std::stringstream& l_stream) {
		l_stream >> m_texture >> m_size.x >> m_size.y >> m_rect.x >> m_rect.y;
	}

	void Create(TextureManager* l_textureMgr)
	{
		if (!l_textureMgr->RequireResource(m_texture)){
			std::cout << "! Could not set up the texture: " << m_texture << std::endl;
		}
		else {
			if (l_textureMgr->GetResource(m_texture)){
				m_sprite.setTexture(*l_textureMgr->GetResource(m_texture));
				m_sprite.setTextureRect(sf::IntRect(m_rect.x, m_rect.y, m_size.x, m_size.y));
				m_sprite.setOrigin(m_size.x / 2.0, m_size.y / 2.0);
			}
		}
	}

	void UpdatePosition(const sf::Vector2f& l_vec, const float& angle) {
		m_sprite.setPosition(l_vec);
		m_sprite.setRotation(converter::radToDeg<double>(angle));
	}

	const sf::Vector2u& GetSize() {
		return m_size;
	}

	void Draw(sf::RenderWindow* l_wind) {
		//ADD ERROR !!!!
		l_wind->draw(m_sprite);
	}
private:
	sf::Sprite m_sprite;
	std::string m_texture;
	sf::Vector2u m_size;
	sf::Vector2f m_rect;
};