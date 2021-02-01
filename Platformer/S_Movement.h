#pragma once
#include "S_Base.h"

enum class Axis { x, y };

class Map;

class S_Movement : public S_Base {
public:
	S_Movement(SystemManager* l_systemMgr);
	~S_Movement();

	void Update(float l_dT);
	void HandleEvent(const EntityId& l_entity, const EntityEvent& l_event);
	void Notify(const Message& l_message);

private:
	void SetDirection(const EntityId& l_entity, const Direction& l_dir);
};