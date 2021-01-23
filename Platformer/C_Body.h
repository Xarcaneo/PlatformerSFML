#pragma once
#include <SFML/Graphics.hpp>
#include <BOX2D/box2d.h>
#include "C_Base.h"
#include "Converter.h"
#include "Directions.h"

class C_Body : public C_Base {
public:
    C_Body() : C_Base(Component::Body), m_world(nullptr) {}
    ~C_Body() {}

    void ReadIn(std::stringstream& l_stream) {
        l_stream >> m_fixedRotation >> m_type >> m_density
            >> m_friction >> m_restitution >> m_size.x >> m_size.y
            >> m_moveSpeed;
    }

    void CreateBody()
    {
        b2BodyDef BodyDef;
        BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
        BodyDef.fixedRotation = m_fixedRotation;

        if (m_type == 1)
            BodyDef.type = b2_staticBody;
        else if (m_type == 2)
            BodyDef.type = b2_dynamicBody;
        else if (m_type == 3)
            BodyDef.type = b2_kinematicBody;

        this->m_body = m_world->CreateBody(&BodyDef);
        this->m_body->SetLinearDamping(1);
        this->m_body->SetGravityScale(2.0f);

        b2PolygonShape box2d_shape;
        box2d_shape.SetAsBox(converter::pixelsToMeters<double>(m_size.x / 2.0),
            converter::pixelsToMeters<double>(m_size.y / 2.0));
        b2FixtureDef FixtureDef;
        FixtureDef.density = m_density;
        FixtureDef.friction = m_friction;
        FixtureDef.restitution = m_restitution;
        FixtureDef.shape = &box2d_shape;
        FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
        this->m_body->CreateFixture(&FixtureDef);
    }

    void SetGlobalWorld(b2World* l_world) { this->m_world = l_world; }

    void SetPosition(sf::Vector2f position)
    {
        this->m_position.x = converter::pixelsToMeters(position.x);
        this->m_position.y = converter::pixelsToMeters(position.y);

        m_body->SetTransform(b2Vec2(position.x, position.y), m_body->GetAngle());
    }

    const sf::Vector2f GetPosition() {
        sf::Vector2f pos;
        pos.x = m_body->GetPosition().x;
        pos.y = m_body->GetPosition().y;

        return pos;
    }

    void Move(const Direction& l_dir)
    {
        b2Vec2 vel = m_body->GetLinearVelocity();

        switch (l_dir)
        {
        case Direction::Left: vel.x = b2Max(vel.x - 0.1f, -m_moveSpeed); break;
        case Direction::Right: vel.x = b2Min(vel.x + 0.1f, m_moveSpeed); break;
        } 

        std::cout << vel.x << std::endl;

        m_body->SetLinearVelocity(vel);
    }

private:
    b2Body* m_body;
    b2World* m_world;
    bool m_fixedRotation;
    sf::Vector2f m_position;
    int m_type;
    float m_density;
    float m_friction;
    float m_restitution;
    sf::Vector2f m_size;
    float m_moveSpeed;
};