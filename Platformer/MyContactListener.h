#pragma once
#include <BOX2D/box2d.h>
#include "C_Body.h"

class MyContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) {

        b2Body* bodyUserDataA = contact->GetFixtureA()->GetBody();
        if (bodyUserDataA != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataA->GetUserData().pointer;
            body->StartContact();
        }

        b2Body* bodyUserDataB = contact->GetFixtureB()->GetBody();
        if (bodyUserDataB != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataB->GetUserData().pointer;
            body->StartContact();
        }

    }

    void EndContact(b2Contact* contact) {

        b2Body* bodyUserDataA = contact->GetFixtureA()->GetBody();
        if (bodyUserDataA != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataA->GetUserData().pointer;
            body->EndContact();
        }

        b2Body* bodyUserDataB = contact->GetFixtureB()->GetBody();
        if (bodyUserDataB != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataB->GetUserData().pointer;
            body->EndContact();
        }
    }
};