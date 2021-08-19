#pragma once
#include "EnemyState.h"
#include "EnemyBehaviorComponent.h"

class NearEnemyIdle : public EnemyState
{
public:
	NearEnemyIdle(EnemyBehaviorComponent* owner);
	~NearEnemyIdle();
	EnemyStateEnum Update(float deltaTime)override;
	void		   OnEnter()override;
	void           OnExit() override;
};