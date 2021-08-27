#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyLookAround : public EnemyState
{
public:
	NearEnemyLookAround(class EnemyBehaviorComponent* owner);
	~NearEnemyLookAround();
	EnemyStateEnum Update(float deltaTime) override;
	void           OnEnter() override;
	void           OnExit()  override;

};