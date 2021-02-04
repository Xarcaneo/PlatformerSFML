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

            if (contact->GetFixtureA()->GetUserData().pointer == 3)
                body->StartContact();
        }

        b2Body* bodyUserDataB = contact->GetFixtureB()->GetBody();
        if (bodyUserDataB != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataB->GetUserData().pointer;

            if (contact->GetFixtureB()->GetUserData().pointer == 3)
                body->StartContact();
        }

    }

    void EndContact(b2Contact* contact) {

        b2Body* bodyUserDataA = contact->GetFixtureA()->GetBody();
        if (bodyUserDataA != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataA->GetUserData().pointer;

            if (contact->GetFixtureA()->GetUserData().pointer == 3)
                body->EndContact();
        }

        b2Body* bodyUserDataB = contact->GetFixtureB()->GetBody();
        if (bodyUserDataB != NULL)
        {
            C_Body* body = (C_Body*)bodyUserDataB->GetUserData().pointer;

            if (contact->GetFixtureB()->GetUserData().pointer == 3)
                body->EndContact();
        }
    }
};